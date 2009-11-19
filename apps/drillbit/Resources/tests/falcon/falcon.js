	describe("Falcon Tests",
	{
	test_smoke: function()
	{
		value_of(window.javascript_var).should_be('variable from javascript');
	},
	test_inline: function()
	{
		value_of(window.inline_test_result).should_be('Added manually');
	},
	test_falcon_func: function()
	{
		value_of(window.FalconFunc).should_be_function()
		value_of(window.falcon_func_result).should_be('Value from falcon');
	},
   test_falcon_func2: function()
   {
      value_of(window.falcon_func_result2).should_be('Value returned from a function');
   },
	test_funcall: function()
	{
		value_of(window.funcall_result).should_be('Value returned from a function');
	},
	test_doc_inline: function()
	{
		value_of(window.doc_inline_result).should_be('Document Div Content');
	},
	test_external_file: function()
	{
		value_of(window.external_test_result).should_be('Document Div Content');
	},
	test_require_file_module: function()
	{
		value_of(window.external_import).should_be('<hello world>');
	},
	test_javascript_to_falcon: function()
	{
		value_of(window.javascript_to_falcon_result).should_be("Hello: from falcon");
	},
	test_falcon_int: function()
	{
		value_of(window.number_from_falcon).should_be(1);
	},
	test_falcon_int: function()
	{
		value_of(window.number_from_falcon).should_be(1);
	},
	test_falcon_float: function()
	{
		value_of(window.float_from_falcon).should_be(1.123);
	},
	test_falcon_null: function()
	{
		value_of(window.null_from_falcon).should_be(null);
	},

	
	test_falcon_dict: function()
	{
		value_of(window.dict_from_falcon).should_be_object();
		value_of(window.dict_from_falcon.length).should_be(3);
		value_of(window.dict_from_falcon['A']).should_be(0);
		value_of(window.dict_from_falcon['B']).should_be(1.1);
		value_of(window.dict_from_falcon['C']).should_be("c string");

		window.dict_from_falcon['D'] = 100;
		value_of(window.dict_from_falcon['D']).should_be(100);
		
		window.dict_from_falcon['A'] = "Another";
		value_of(window.dict_from_falcon['A']).should_be("Another");
		
	},
	test_falcon_object: function()
	{
		value_of(window.object_from_falcon).should_be_object();
		value_of(window.object_from_falcon.num).should_be(1);
		value_of(window.object_from_falcon.string).should_be("string property");
		value_of(window.object_from_falcon.val).should_be("inst");
	},
	test_falcon_method: function()
	{
		value_of(window.method_from_falcon).should_be_function();
		value_of(window.method_from_falcon()).should_be("A test: inst");
	},

	});
