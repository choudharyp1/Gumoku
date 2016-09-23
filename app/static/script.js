/**
 * Created by Pratik on 9/20/2016.
 */

$(document).ready(function () {
    $('#gomoku td').click(function () {
    	if ($(this).attr('id') != "lol") {
    		$(this).addClass('botimage')
	        console.log($(this).attr('id'))
	        window.location.href='/play/' + $(this).attr('id');
    	}
        return false;
    })
})