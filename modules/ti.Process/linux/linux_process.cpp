
#include "linux_process.h"
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

namespace ti
{	
	// Parts derived from Poco::Process_UNIX
	AutoPtr<LinuxProcess> LinuxProcess::currentProcess = new LinuxProcess();
	
	/*static*/
	AutoPtr<LinuxProcess> LinuxProcess::GetCurrentProcess()
	{
		return currentProcess;
	}
	
	LinuxProcess::LinuxProcess(SharedKList args, SharedKObject environment, AutoOutputPipe stdin, AutoInputPipe stdout, AutoInputPipe stderr) :
		Process(args, environment, stdin, stdout, stderr),
		running(false),
		complete(false),
		current(false),
		pid(-1),
		exitCode(-1),
		logger(Logger::Get("Process.LinuxProcess"))
	{
	}
	
	LinuxProcess::LinuxProcess() :
		Process(),
		running(true), complete(false), current(true),
		logger(Logger::Get("Process.LinuxProcess"))
	{
		pid = getpid();
		
		for (int i = 0; environ[i] != NULL; i++)
		{
			std::string entry = environ[i];
			std::string key = entry.substr(0, entry.find("="));
			std::string val = entry.substr(entry.find("=")+1);
			
			SetEnvironment(key.c_str(), val.c_str());
		}
		
		SharedApplication app = Host::GetInstance()->GetApplication();
		for (size_t i = 0; i < app->arguments.size(); i++)
		{
			args->Append(Value::NewString(app->arguments.at(i)));
		}
	}
	
	LinuxProcess::~LinuxProcess()
	{
		if (!current)
		{
			try {
				Terminate();
			} catch (ValueException &ve) {
				logger->Error(ve.what());
			}
			
			if (this->exitMonitorThread.isRunning())
			{
				try
				{
					this->exitMonitorThread.join();
				}
				catch (Poco::Exception& e)
				{
					logger->Error(
						"Exception while try to join with exit monitor thread: %s",
						e.displayText().c_str());
				}
			}

			delete exitMonitorAdapter;
		}
	}
	
	void LinuxProcess::Launch(bool async)
	{
		pid = fork();
		if (pid < 0)
		{
			throw ValueException::FromFormat("Cannot fork process for %s", args->At(0)->ToString());		
		}
		else if (pid == 0)
		{
			// setup redirection
			dup2(GetStdin()->GetReadHandle(), STDIN_FILENO);
			GetStdin()->Close();
			// outPipe and errPipe may be the same, so we dup first and close later
			dup2(GetStdout()->GetWriteHandle(), STDOUT_FILENO);
			dup2(GetStderr()->GetWriteHandle(), STDERR_FILENO);
			GetStdout()->Close();
			GetStderr()->Close();
			
			// close all open file descriptors other than stdin, stdout, stderr
			for (int i = 3; i < getdtablesize(); ++i)
				close(i);
			
			size_t i = 0;
			char** argv = new char*[args->Size() + 1];
			//argv[i++] = const_cast<char*>(command.c_str());
			for (;i < args->Size(); i++)
			{
				argv[i] = const_cast<char*>(args->At(i)->ToString());
			}
			argv[i] = NULL;
			
			SharedStringList envNames = environment->GetPropertyNames();
			for (i = 0; i < envNames->size(); i++)
			{
				setenv(envNames->at(i)->c_str(),
					environment->Get(envNames->at(i)->c_str())->ToString(), 1);
			}

			const char *command = args->At(0)->ToString();
			execvp(command, argv);
			_exit(72);
		}
		
		close(GetStdin()->GetReadHandle());
		close(GetStdout()->GetWriteHandle());
		close(GetStderr()->GetWriteHandle());
			
		this->running = true;
		
		// setup threads which can read output and also monitor the exit
		GetStdout()->StartMonitor();
		GetStderr()->StartMonitor();
		this->exitMonitorAdapter = 
			new Poco::RunnableAdapter<LinuxProcess>(*this, &LinuxProcess::ExitMonitor);
		this->exitMonitorThread.start(*exitMonitorAdapter);
	}
	
	int LinuxProcess::GetPID()
	{
		return pid;
	}
	
	void LinuxProcess::SendSignal(int signal)
	{
		if (running)
		{
			if (kill(pid, signal) != 0)
			{
				switch (errno)
				{
				case ESRCH:
					throw ValueException::FromString("Couldn't find process");
				case EPERM:
					throw ValueException::FromString("Invalid permissions for terminating process");
				default:
					throw ValueException::FromFormat("Couldn't send signal: %d to process", signal);
				}
			}
		}
	}
	
	void LinuxProcess::Kill()
	{
		SendSignal(SIGKILL);
	}
	
	void LinuxProcess::Terminate()
	{
		SendSignal(SIGINT);
	}
	
	void LinuxProcess::Restart()
	{
		//TODO
	}

	void LinuxProcess::Restart(SharedKObject env, AutoOutputPipe stdin, AutoInputPipe stdout, AutoInputPipe stderr)
	{
		//TODO
	}

	bool LinuxProcess::IsRunning()
	{
		return running;
	}
	
	void LinuxProcess::ExitMonitor()
	{
		
		int status;
		int rc;
		do
		{
			rc = waitpid(this->pid, &status, 0);
		}
		while (rc < 0 && errno == EINTR);
		if (rc != this->pid)
		{
			throw ValueException::FromFormat("Cannot wait for process: %d", this->pid);
		}
		
		this->running = false;
		this->complete = true;
		this->exitCode = WEXITSTATUS(status);
		Process::Exited();
	}
}