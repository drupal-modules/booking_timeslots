// $Id$

This module is not fully finished.
There are some pending tasks to do before there will be some official release.

1.
Copy theme files from theme directory into your theme folder.
Change them as you wish.

In calendar-day.tpl.php you need to change following lines:
    define('AVAIL_SLOTS', 1); // CHANGE here to set limit if you have one or many slots available in the same time
    define('EVENT_TIME', 1); // for HOW LONG each event should be booked (please put number of half hours, 2 = hour, 3 = hour and half, etc.)
to define number of slots and time of each event
If the templates will not appear under you calendar view, go to 'Theme: Information' section and rescan your files (or try to clear cache Views).

TODO: Later those settings should be moved to some settings page.

