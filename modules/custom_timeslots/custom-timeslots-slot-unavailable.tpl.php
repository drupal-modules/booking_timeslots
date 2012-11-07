<?php
/**
 * @file
 *   Default theme implementation to format the available slot.
 * 
 * @version
 * 
 * Copy this file in your theme directory to create a custom themed footer.
 *
 * Available variables:
 * - $node: node object
 * - $link: link to the booking timeslot
 * - $title: language object
 *
 * @see template_preprocess_custom_timeslots_slot_unavailable()
 */
?>
<?php
print '<center>' . l(t('Slot unavailable.'), "node/$node->nid") . '</center>';
print node_view($node, TRUE, TRUE);
?>

