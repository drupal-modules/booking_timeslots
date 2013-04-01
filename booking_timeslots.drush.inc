<?php


/**
 * @file
 * Drush commands for backup and migrate.
 */
 
/**
 * Implementation of hook_drush_command().
 */
function booking_timeslots_drush_command() {
  
  $items['generate-bookings'] = array(
    'callback'           => 'booking_timeslots_drush_generate_bookings',
    'description'        => t('Generates random booking entities as well as booking slot configurations for entities.'),
    'examples'           => array(
      'drush generate-bookings 10' => t('Generates specific number of random booking entities per content type.') 
    ),
    'arguments'          => array(  
      'number'             => "Required. Number of bookings to generate per venue.",
    ),
    'required-arguments' => TRUE,
  );
  
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function booking_timeslots_drush_help($section) {
  switch ($section) {
    case 'drush:generate-entities':
      return t("Booking Entities Generator");
  }
}

/**
 * @TODO
 */
function booking_timeslots_drush_generate_bookings($number = 10) {
  
  require_once 'booking_timeslots.booking.inc';

  // Fetching list of available venues
  $venue_ids        = array_keys(db_query("select nid from {node} where type = 'venue'") -> fetchAllAssoc ('nid'));
  $num_venues       = count($venue_ids);
  
  // Fetching list of available facilities
  $facility_ids     = array_keys(db_query("select nid from {node} where type = 'facility'") -> fetchAllAssoc ('nid'));
  $num_facilities   = count($facility_ids);
  
  $slots_config_ids = array();
  
  // Generating booking slot configuration for every facility in every venue
  for ($v = 0; $v < $num_venues; $v++) {
    for ($f = 0; $f < $num_facilities; $f++) {
      $slots_config_ids[$v][$f] = db_insert('booking_slot_config') -> fields(array(
        'vid'          => $venue_ids[$v],
        'fid'          => $facility_ids[$f],
        'day_of_week'  => rand(1, 7),
        'hour_start'   => rand(0, 14),
        'min_start'    => rand(0, 59),
        'hour_end'     => rand(15, 20),
        'min_end'      => rand(0, 59),
        'price'        => rand(1, 50),
        'duration'     => rand(1, 3),
        'capacity'     => rand(1, 20),
        'closed'       => rand(0, 1),
      )) -> execute();
    }
  }
  
  
  // Generating specified number of bookings for every facility in every venue
  for ($v = 0; $v < $num_venues; $v++) {
    for ($f = 0; $f < $num_facilities; $f++) {
      for ($i = 0; $i < $number; $i++) {
        $booking = new Booking;
        $booking -> cid        = $slots_config_ids[$v][$f];
        $booking -> created    = date('U', strtotime( '+'.mt_rand(0,30).' days'));
        $booking -> slot_time  = date('Y-' . sprintf("%02s", rand(1, 12)) . '-' . sprintf("%02s", rand(1, 28)) . ' ' . sprintf("%02s", rand(0, 23)) . ':' . sprintf("%02s", rand(0, 59)) . ':' . sprintf("%02s", rand(0, 59)));
        $booking -> hour_start = rand(0, 14);
        $booking -> min_start  = rand(0, 59);
        $booking -> duration   = rand(30, 60);
        $booking -> players    = rand(1, 20);
        $booking -> total      = rand(15, 100);
        $booking -> deposit    = rand(0, 10);
        $booking -> tax        = rand(1, 2);
        $booking -> save();
      }
    }
  }

}

