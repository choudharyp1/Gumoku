/**
 * Created by Pratik on 9/20/2016.
 */

$(document).ready(function () {

	$('#stop').hide();
	$('#devariation').hide();

    $('#gomoku td').click(function () {
    	if ($(this).attr('id') != "lol") {
    		$(this).addClass('botimage')
	        console.log($(this).attr('id'))
	        window.location.href='/play/' + $(this).attr('id');
    	}
        return false;
    })
	$('#start').click(function () {
		$(this).hide();
		$('#stop').show();
	});
	$('#stop').click(function () {
		$(this).hide();
		$('#start').show();
	});
	$('#variation').click(function () {
		$(this).hide();
		$('devariation').show();
	});
	$('#devariation').click(function () {
		$(this).hide();
		$('#variation').show();
	});
})