<?php
/**
 * @file
 * booking_timeslots_example.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function booking_timeslots_example_taxonomy_default_vocabularies() {
  return array(
    'booking_timeslots_example_categories' => array(
      'name' => 'Booking Timeslots Example Categories',
      'machine_name' => 'booking_timeslots_example_categories',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
