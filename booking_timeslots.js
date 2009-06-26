if (Drupal.jsEnabled) {
  // When the DOM is ready, try an AJAX content load
  
  $(document).ready(function() {
    // Start date
    input_obj = $('.container-inline-date').find("input");

    if (!(input_obj.eq(0).datepicker === undefined))
    input_obj.eq(0).datepicker({ 
        showStatus: true,
        onSelect: function(date) { 
            input_obj.eq(2).val(date)
        } 
    });

    // Date To
    if (!(input_obj.eq(2).datepicker === undefined))
    input_obj.eq(2).datepicker({ 
        showStatus: true,
        onSelect: function(date) { 
            input_obj.eq(0).val(date)
        } 
    });
})
};

