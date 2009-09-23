<?php
/**
 * @file
 *   Default theme implementation to format the available slot.
 * 
 * @version
 *   $Id$
 * 
 * Copy this file in your theme directory to create a custom themed footer.
 *
 * Available variables:
 * - $node: node object
 * - $link: link to the booking timeslot
 * - $title: language object
 *
 * @see template_preprocess_custom_timeslots_slot_available()
 */
?>
<?php
print "<a href='$link'>" . $title . '</a>';
?>

