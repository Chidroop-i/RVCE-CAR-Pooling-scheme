/*
 * Just using some Javascript to automatically
 * demonstrate the hover effect.
 */
$(function() {
  var $menuClusterToggler = $('#menuClusterToggler'),
      demoTimer;
  
  $menuClusterToggler.one('mouseenter', function() {
      clearTimeout(demoTimer);
  });
  
  demoTimer = setTimeout(function() {
    $menuClusterToggler.addClass('is-hovered');
    
    demoTimer = setTimeout(function() {
      $menuClusterToggler.removeClass('is-hovered');
    }, 2000);
  }, 2000);
});