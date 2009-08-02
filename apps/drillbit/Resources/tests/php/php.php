<?php

include_once 'foo.php';

function require_file_module()
{
    $f = new Foo();
    return $f->bar();
}
