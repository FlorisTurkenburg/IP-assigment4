<?php
// Initialize Smarty.
$topdir=realpath(".");
include('smarty/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir($topdir.'/smarty/templates');
$smarty->setCompileDir($topdir.'/smarty/templates_c');
$smarty->setCacheDir($topdir.'/smarty/cache');
$smarty->setConfigDir($topdir.'/smarty/configs');

// Load config and assign it to Smarty variables.
include('config.php');
$smarty->assign('phpbase', $WEB_BASEPHP);
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('hotelgwaddress', $HOTELGW_ADDRESS);
$smarty->assign('hotelgwport', $HOTELGW_PORT);
$smarty->assign('paperaddress', $PAPER_ADDRESS);
$smarty->assign('paperport', $PAPER_PORT);

//for debug purposes
function console_log( $data ){
  echo '<script>';
  echo 'console.log('. json_encode( $data ) .')';
  echo '</script>';
}

//connect to hotel gateway
$fp = fsockopen($HOTELGW_ADDRESS, $HOTELGW_PORT, $errno, $errstr);
if (!$fp) {
    echo "$errstr ($errno)<br />\n";
    console_log("error");
} else {
    $hotelgw = fgets($fp,9);
    console_log("first hotelgw");
    console_log($hotelgw);
    fwrite($fp, "l\n");
    $response = fgets($fp);
    $secondgw = fgets($fp,9);
    fclose($fp);
}

$response = substr($response, 0, -1); //trim the new line char at the end
$response = explode("\t", $response);

$smarty->assign('response', $response);


// Display page.
$smarty->display('tpl/hotellist.html');
?>
