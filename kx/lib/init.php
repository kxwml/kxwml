<?php

header('Content-tyle:text/html;charset=utf8');
define('ROOT' , dirname(__DIR__));
require  (ROOT . '\lib\mysql.php');
require  (ROOT . '\lib\func.php');

$_GET = _addslashes($_GET);
$_POST = _addslashes($_POST);
$_COOKIE = _addslashes($_COOKIE);
?>
