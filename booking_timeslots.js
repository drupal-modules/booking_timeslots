if (Drupal.jsEnabled) {
  // When the DOM is ready, try an AJAX content load
  
  $(document).ready(function() {
    // Start date
    $('.container-inline-date').find("input").eq(0).datepicker({ 
        showStatus: true,
        onSelect: function(date) { 
            $('.container-inline-date').find("input").eq(2).val(date)
        } 
    });

    // End date
    $('.container-inline-date').find("input").eq(2).datepicker({ 
        showStatus: true,
        onSelect: function(date) { 
            $('.container-inline-date').find("input").eq(0).val(date)
        } 
    });
})
};

