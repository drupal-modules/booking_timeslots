<?php

class Booking extends Entity {
   
  public
    $cid,
    $created,
    $uid,
    $primary_id,
    $secondary_id,
    $tertiary_id,
    $quaternary_id,
    $slot_time,
    $duration,
    $players,
    $total,
    $deposit,
    $tax,
    $notes;

  public function __construct($values = array()) {
    // Following construction mean that the booking_timeslots_booking_* handlers will be used to manage this entity
    parent::__construct($values, 'booking'); 
  }
  
}

class BookingController extends EntityAPIControllerExportable  {
  
  public function create(array $values = array()) {

    $values === NULL ?: $values = array();
    $values += array(
      'created'     => REQUEST_TIME,
     // 'slot_time'     => date('Y-' . sprintf("%02s", rand(1, 12)) . '-' . sprintf("%02s", rand(1, 28)) . ' ' . sprintf("%02s", rand(0, 23)) . ':' . sprintf("%02s", rand(0, 59)) . ':' . sprintf("%02s", rand(0, 59))),
    );
   //var_dump($_POST);
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('booking', $entity);
    
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
 
 /*
  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // Ensure that uid is taken from the {node} table,
    // alias timestamp to revision_timestamp and add revision_uid.
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    // $fields =& $query->getFields();
    //$query->addField('revision', 'timestamp', 'revision_timestamp');
    //$fields['uid']['table'] = 'base';
    //$query->addField('revision', 'uid', 'revision_uid');
    return $query;
  }
  */

  public function load($ids = array(), $conditions = array()) {
    return parent::load($ids, $conditions);
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    return parent::save($entity, $transaction);
  }
  
}

class BookingUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this -> path]['description'] = 'Manage bookings.';
    return $items;
  }

}
