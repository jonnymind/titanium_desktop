(function()
{
	var createWorker = Titanium.Worker.createWorker;
	
	Titanium.API.register("ti.UI.window.page.init",function(name,event)
	{
		var window = event.scope;
		
		// hook into the createWorker and make sure we pass in the window
		// object as the first parameter

		window.Titanium.Worker.createWorker = function(value)
		{
			return createWorker(window,String(value),typeof(value)=='function');
		};
	});
	
})();


