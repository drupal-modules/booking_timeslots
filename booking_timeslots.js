if (Drupal.jsEnabled) {
  // When the DOM is ready, try an AJAX content load
  
  $(document).ready(function() {
    // Start date
    $("#edit-field-party-datetime-0-value-datepicker-popup-0").datepicker({ 
	showStatus: true,
	onSelect: function(date) { 
	    $("#edit-field-party-datetime-0-value2-datepicker-popup-0").val(date) // TODO: change to some universal selector (XPath)
	} 
    });

    // End date
    $("#edit-field-party-datetime-0-value2-datepicker-popup-0").datepicker({ 
	showStatus: true,
	onSelect: function(date) { 
	    $("#edit-field-party-datetime-0-value-datepicker-popup-0").val(date) // TODO: change to some universal selector (XPath)
	} 
    });

    $('.container-inline-date').parent().hide(); // hide input date form

})
};

