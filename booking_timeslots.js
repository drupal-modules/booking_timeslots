(function ($) {
  
  $(document).ready(function() {
    $('.container-inline-date').find("input").eq(0).change(function(){$('.container-inline-date').find("input").eq(2).val(this.value)});
    $('.container-inline-date').find("input").eq(2).change(function(){$('.container-inline-date').find("input").eq(0).val(this.value)});
  })
  
})(jQuery);
