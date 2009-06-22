describe("async worker tests",
{
	test_worker_as_async: function(result)
	{
		var worker = Titanium.Worker.createWorker(function()
		{
			postMessage("123");
		});
		
		worker.onmessage = function(v)
		{
			worker.terminate();
			clearTimeout(timer);
			try
			{
				value_of(v).should_be('123');
				result.passed();
			}
			catch(e)
			{
				result.failed(e);
			}
		};
		
		var timer = setTimeout(function()
		{
			result.failed("timed out");
		},2000);
		
	},

	test_worker_with_titanium_as_async: function(result)
	{
		var version = Titanium.version;
		
		var worker = Titanium.Worker.createWorker(function()
		{
			postMessage(Titanium.version);
		});
		
		worker.onmessage = function(v)
		{
			worker.terminate();
			clearTimeout(timer);
			try
			{
				value_of(v).should_be(version);
				result.passed();
			}
			catch(e)
			{
				result.failed(e);
			}
		};
		
		var timer = setTimeout(function()
		{
			result.failed("timed out");
		},2000);
		
	},
	test_worker_as_external_file_as_async: function(result)
	{
		var worker = Titanium.Worker.createWorker('test.js');
		
		worker.onmessage = function(v)
		{
			worker.terminate();
			clearTimeout(timer);
			try
			{
				value_of(v).should_be('you said: 123');
				result.passed();
			}
			catch(e)
			{
				result.failed(e);
			}
		};
		
		var timer = setTimeout(function()
		{
			result.failed("timed out");
		},2000);
		
		worker.postMessage("123");
	},

	test_worker_as_external_file_with_import_as_async: function(result)
	{
		var worker = Titanium.Worker.createWorker('test2.js');
		
		worker.onmessage = function(v)
		{
			worker.terminate();
			clearTimeout(timer);
			try
			{
				value_of(v).should_be('you said: 123');
				result.passed();
			}
			catch(e)
			{
				result.failed(e);
			}
		};
		
		var timer = setTimeout(function()
		{
			result.failed("timed out");
		},2000);
		
		worker.postMessage("123");
	},
	
});


