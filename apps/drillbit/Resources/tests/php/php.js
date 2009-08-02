describe("PHP Tests",
{
    test_require_file_module: function()
    {
        value_of(window.require_file_module_result).should_be('hello,world');
    },
});
