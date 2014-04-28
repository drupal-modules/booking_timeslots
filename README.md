Booking Time Slots
==================

Drupal project page: http://drupal.org/project/booking_timeslots.

GitHub project page: https://github.com/kenorb/booking_timeslots.

Requirements
------------

 - PHP 5.3
 
Installation
------------

Please download and enable booking_timeslots module manually or via drush:
  drush -y dl booking_timeslots
  drush -y en booking_timeslots

Configuration
-------------

To properly configure the module, please look at the tutorial available at /admin/config/booking_timeslots/tutorial.

Demo
----

1. Enable booking_timeslots_example module.
2. Go to "Add content" and then to "Booking Timeslots Example Venue". Enter any title and save node. Now go to the "Opening Hours" tab.
3. Click on some day of week to add venue opening hours.
4. Enter start time e.g. 08:00 and end time e.g. 20:00. It will restrict facilities' and classes' opening hours to be lay between specified hours.
5. Go to "Add content" and then to "Booking Timeslots Example Facility". Enter any title and select previously created Venue. Save the node.
7. Go to the "Opening Hours" tab of the facility.
8. Click on the day when previously created venue is open and specify facility opening hours (must lay between venue opening hours), pricing(mandatory) and other things.
9. You may finally open the venue and go to "Schedule" tab to display calendar and be able to book slots.
10. If you create "Booking Timeslots Example Class", you may relate it with the facility. This will allow you to create bookable slots of custom length (user must book the whole slot instead of selecting custom length).
11. If you create "Booking Timeslots Example Instructor", you may relate it with the class to enable filtering classes by instructor (you won't be able to create slots per instructor so 4th level content type acts like classes filter).

Those steps should allow you to book timeslots.
