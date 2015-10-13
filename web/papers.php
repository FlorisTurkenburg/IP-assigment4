<?php
    require('index.php');

    echo "WTF!";
?>

<script src="tpl/js/jquery.js"></script>
<!-- {block name="content"} -->
<div id="paperlist"></div>

<script>
        $.get("http://127.0.0.1/~floris/cgi-bin/papers.cgi", dataType="json" )
            .done(function(data) {
                var obj = jQuery.parseJSON(data);
                var list = "";
                jQuery.each(obj, function(index, data) {
                    list += "<li class=\"paper\"><span class=\"id\">["+data.id+"]</span>&emsp;<span class=\"author\">"+data.author+"</span>&emsp;<a class=\"title\">"+data.title+"</a></li>";
                });
                
                $('#paperlist').html("<ul class=\"paperlist\">"+list+"</ul>");

            }
        );
    
</script>
<!-- {/block} -->
<?php

    // $smarty->display('extends:tpl/index.html|papers.php');
    // $smarty->display('tpl/papers.html');
?>


