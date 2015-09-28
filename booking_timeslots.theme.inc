<?php
/**
 * @file
 *   Theme include file
 *
 * @version
 *
 * @developers
 *   Rafal Wieczorek <kenorb+nospam@gmail.com>
 */

/**
 * Theme preprocess callback
 *
 * Format an calendar day node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_day(&$vars) {
  return template_preprocess_booking_timeslots_day_overlap($vars);
}

/**
 * Theme preprocess callback
 *
 * Format an calendar day node for display.
 *  See: template_preprocess_calendar($vars);
 *
 * Even though this function is used to preprocess calendars with no overlapping
 * items, calendars should have overlapping disabled.
 */
function template_preprocess_booking_timeslots_day_overlap(&$vars) {

 $day = $vars['view'] -> argument['date_argument'] -> argument;

 $now = new DateTime($day);

 $time_start     = date ('Y-m-d H:i:s', $now -> getTimestamp());
 $time_end       = date ('Y-m-d H:i:s', $now -> getTimestamp() + 86399);

 template_preprocess_booking_timeslots_general($vars, 'day', $time_start, $time_end);
}

/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_week_overlap(&$vars) {

  $week = $vars['view'] -> argument['date_argument'] -> argument;

  list($time_start, $time_end) = booking_timeslots_week_to_start_end_date ($week);

  foreach ($vars['day_names'] as $i => &$row) {
    $day_no   = date ('d', strtotime($time_start) + $i * 86400);
    $month_no = date ('m', strtotime($time_start) + $i * 86400);
    $row['data'] = $row['data'] . '<span class="day-num">' . $day_no . '</span>'; // . '<span class="month-num">' . $month_no . '</span> ';
  }

  template_preprocess_booking_timeslots_general($vars, 'week', $time_start, $time_end);
}


/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_week(&$vars) {
  template_preprocess_booking_timeslots_week_overlap($vars);
}

function template_preprocess_booking_timeslots_general(&$vars, $view_mode, $start_time, $end_time) {

  drupal_add_js(array('booking_timeslots_calendar_granularity' => booking_timeslots_get_calendar_granularity()), 'setting');

  $configuration       = booking_timeslots_get_configuration();
  $configuration_valid = booking_timeslots_check_configuration_validity();

  if (!$configuration_valid) {
    return;
  }

  $grid_granularity = booking_timeslots_get_calendar_granularity();

  $vid  = $vars['view'] -> argument['nid'] -> argument;
  $fid  = @$vars['view'] -> exposed_raw_input['secondaries'];
  $cid  = @$vars['view'] -> exposed_raw_input['tertiaries'];

  $vars['view_time_start'] = $start_time;
  $vars['view_time_end']   = $end_time;

  if (is_numeric($cid) && !is_numeric($fid)) {
    $field_class_facility = $configuration['ct_field_3_2'];
    $fid = node_load($cid) -> $field_class_facility['und'][0]['nid'];
  }

  if ($fid == 'All') {
    $fid  = $vars['view']->exposed_input['secondaries'];
  }

  $vars['granularity'] = booking_timeslots_filters_get_duration($vars['view']);

  drupal_add_css(drupal_get_path('module', 'booking_timeslots') .'/css/booking_timeslots.css');

  $data_matrix = NULL;

  if(is_array($fid))
  {
    if($view_mode == 'month')
    {

      $data_matrix = booking_timeslots_get_availability_data_matrix($vid, $fid[0], $cid, $view_mode, $start_time, $end_time, @$vars['start_times'], $vars['granularity'], $grid_granularity);
    }
    else
    {
      $instance_data = array();
      $temp_matrix = array();

      foreach($fid as $facility_id)
      {
        $temp_matrix[] = booking_timeslots_get_availability_data_matrix($vid, $facility_id, $cid, $view_mode, $start_time, $end_time, $vars['start_times'], $vars['granularity'], $grid_granularity);
      }

      $new_matrix = array_shift($temp_matrix); // sets first matrix as 'base'

      // invalidation for the slots that are unbookable foro given duration
      foreach($temp_matrix as $matrix)
      {
        $current_matrix = array();

        foreach($matrix['data'] as $day => $mdata)
        {
          $invalid = array();
          $current_matrix[$day] = false;

          // check which days we can actually invalidate
          foreach($mdata as $hour => $hour_data)
          {
            if (isset($hour_data['length']))
            {
              $current_matrix[$day] = true;
            }
          }


          // invalidates every slot thats unbookable for current duration
          if ($new_matrix['data'][$day])
          foreach($new_matrix['data'][$day] as $hour => $hour_data)
          {
            if($current_matrix[$day])
            {
              if (isset($hour_data['type']) && $hour_data['type'] == 'green_box' && !is_numeric($cid) && !booking_timeslots_helper_check_duration_available($hour_data['affected_settings'], $vars['granularity']))
              {
                $instance_data[$hour_data['affected_settings']->instance_id] = $hour_data['affected_settings'];
                unset($new_matrix['data'][$day][$hour]);

                $invalid[] = $hour_data['affected_settings']->nid;
                continue;
              }
            }
          }

          // invalidates every slot thats unbookable for current duration
          if ($new_matrix['data'][$day])
          foreach($new_matrix['data'][$day] as $hour => $hour_data)
          {
            if (isset($hour_data['affected_settings'])) {
              if(isset($current_matrix[$day]) && is_object($hour_data['affected_settings']) && in_array($hour_data['affected_settings']->nid, $invalid))
              {
                unset($new_matrix['data'][$day][$hour]);
              }
            }
          }
        }

  //      dsm("LOOP MATRIX");
  //      dsm($matrix['data']);

        foreach($matrix['data'] as $day => $mdata)
        {
          if(is_numeric($day))
          {
            $instance = array();
            $org_hour_data = NULL;


            // find first instance of opened facility
            foreach($new_matrix['data'][$day] as $hour => $hour_data)
            {
              if(isset($hour_data['length']))
              {
                $instance[] = $hour;
              }
            }

            foreach($matrix['data'][$day] as $hour => $hour_data)
            {

              // this hour doesnt exist in orginal instance
              if(!isset( $new_matrix['data'][$day][$hour]['affected_settings'] ))
              {
                // We still have a slot to fill somewhere
                if($org_hour_data != NULL)
                {
                  $h = explode(":", $hour);

                  $mins = $h[0] * 60 + $h[1];

                  if($mins < $org_hour_data['box']['start'] )
                  {
                    $org_hour_data = NULL;
                    continue;
                  }

                  // Fixes the slot start time to not collide with the previous slots.
                  $org_hour_data['box']['start'] = $mins;
                  $org_hour_data['rowspan'] = ($org_hour_data['box']['end'] - $org_hour_data['box']['start']) / $grid_granularity;
                  $org_hour_data['length'] = ($org_hour_data['box']['end'] - $org_hour_data['box']['start']);
                  $org_hour_data['box']['height'] = ($org_hour_data['box']['end'] - $org_hour_data['box']['start']) / $grid_granularity;

                  $hour_data = $org_hour_data;
                  $org_hour_data = NULL;
                }
                else if(isset($hour_data['rowspan']))
                {
                  foreach($instance as $instance_id)
                  {
                    $org_data = $new_matrix['data'][$day][$instance_id]['box'];

                    if ($org_data['start'] < $hour_data['box']['end']) {
                      // Instances collide, shorten the end time to the time it collides with.
                      // @FIXIT: This sometimes cuts box unnecessarily.
                      $hour_data['box']['end'] = $org_data['start'];
                    }
                  }
                }


                $new_matrix['data'][$day][$hour] = $hour_data;
              }
              else // Found instance of that hour on already existing matrix
              {
                if(isset($hour_data['rowspan']))
                {
                  $org_hour_data = $hour_data;
                  $settings = $new_matrix['data'][$day][$hour]['affected_settings'];

                  // If the found slot collides with invalid slot, replace it right away, if not it will be performed on next loop run.
                  if($org_hour_data != NULL && (isset( $new_matrix['data'][$day][$hour]['affected_settings'] ) && in_array($settings->nid, $invalid)))
                  {
                    $new_matrix['data'][$day][$hour] = $org_hour_data;
                    $org_hour_data = NULL;
                  }
                }
              }

              // if we have full slot, check the secondary matrix if there any slots on it, if there are, assign the slot to that hour and add free slots to it
              if($new_matrix['data'][$day][$hour]['capacity'] == 0 && $hour_data['capacity'] > 0)
              {
                $new_matrix['data'][$day][$hour]['capacity'] += $hour_data['capacity'];
                $new_matrix['data'][$day][$hour]['affected_settings'] = $hour_data['affected_settings'];
              }
              else if($new_matrix['data'][$day][$hour]['capacity'] > 0) // there are free slots left, just add the number of free slots from this instance
              {
                $new_matrix['data'][$day][$hour]['capacity'] += $hour_data['capacity'];
              }
            }
          }
        }
      }

      // rebuild the invalidated instances
      // TODO: They need to fill all the places, currently it find the first spot to fill in.
      foreach($instance_data as $oid => $idata)
      {
        $b_time = booking_timeslots_helper_hour_min_to_minutes($idata->start_time_hm);
        $e_time = booking_timeslots_helper_hour_min_to_minutes($idata->end_time_hm);

        $btime = null;
        $etime = null;

        for($i = $b_time; $i < $e_time; $i += $grid_granularity)
        {
          $time = date('H:i', $i * 60);
          if(!isset($new_matrix['data'][$idata->day][$time]['affected_settings']))
          {
            if($btime == null)
            {
              $btime = $i;
            }

            $etime = $i;
          }
          else if($btime != null && isset($new_matrix['data'][$idata->day][$time]['affected_settings']))
          {
            $etime = $i;
            break;
          }
        }

        $length = ($etime - $btime);
        $new_matrix['data'][$idata->day][date('H:i', $btime * 60)] = array(
          'box' => array('start' => $btime, 'end' => $etime, 'height' => ($length / $grid_granularity)),
          'length' => $length,
          'rowspan' => ($length / $grid_granularity),
          'affected_settings' => $idata,
          'capacity' => 1,
          'capacity_max' => 1,
          'type' => 'green_box'
        );
      }

      $data_matrix = $new_matrix;
    }
  }
  else
  {
    $data_matrix = booking_timeslots_get_availability_data_matrix($vid, $fid, $cid, $view_mode, $start_time, $end_time, @$vars['start_times'], $vars['granularity'], $grid_granularity);
  }

//  dsm('DATA_MATRIX');
//  dsm($data_matrix);

  if($view_mode == 'month') {

    $path = current_path();
    $link = url($path, array('absolute' => FALSE));
    $url = explode('/', $link);

    while(array_pop($url) != 'month' && !empty($url))
    {
      // do nothing
    }

    $arg = explode('?', request_uri());
    $get_args = @$arg[1];

    $vars['day_items'] = array();
    $day_counter = 0;
    $org_date = $vars['view'] -> argument['date_argument'] -> argument;

    $month_days = date('t', strtotime( $org_date ));
    $month_start = strtotime( $org_date );
    $month_end = $month_start + $month_days * 86400;
    $today = date('Y-m-d');
    $today_unix = strtotime($today);

    // dsm ($vars);

    foreach($data_matrix['month_data'] as $date => $day) {
      $check_minutes = true;

      $day['url'] = join('/', $url) . ( user_access('administer booking timeslots') ? '/day/' : '/' ) . $date .'?' . $get_args;
      $day['url'] = base64_encode($day['url']);

      if($date == $today) {
        $day['class'] = 'today';
      }
      else if(strtotime($date) < $month_start) {
        $day['class'] = 'past empty';
        $check_minutes = false; // Do not show previous month dates
      }
      else if(strtotime($date) <= $today_unix) {
        $day['class'] = 'past';
        $check_minutes = false; // Do not show past month dates
      }
      else if(strtotime($date) > $month_end) {
        $day['class'] = 'future empty';
        $check_minutes = false; // Do not show next month dates
      }
      else if(strtotime($date) > $today_unix) {
        $day['class'] = 'future';
      }

      if($check_minutes && $day['total_free_minutes'] > 0)
      {
        $day['class'] .= ' calendar-day-green-box';

        $day['free_hours'] = (int)($day['total_free_minutes'] / 60);
        if($day['free_hours'] < 1)
        {
          $day['free_hours'] = $day['total_free_minutes'] . t(' mins');
        }
        else
        {
          $day['free_hours'] = $day['free_hours'] . t(' hrs');
        }
      }
      else {
        // Reset free minutes so they don't show up in places we don't want them to.
        $day['total_free_minutes'] = 0;
      }

      $vars['day_items'][(int)($day_counter / 7)][$date] = $day;

      $day_counter++;
    }
  }

  //dsm($vars['day_items']);



  $start = booking_timeslots_helper_hour_min_to_minutes($data_matrix['daily_settings']['start_time_hm']);

  for ($m = 0; $m < 1440; $m += $grid_granularity) {
    $hour  = substr ('0' . ((int)($m / 60)), -2);
    $min   = substr ('0' . ((int)$m % 60), -2);
    $date  = $hour . ':' . $min;

    $times [] = $date;
  }

  $vars['view'] -> date_info -> style_groupby_times = $times;
  $vars['start_times'] = $times;



  $html_item_day_closed = <<<EOT
    <div class="views-field views-field-created" style="text-align: center">
      <span class="field-content">
        %s
      </span>
    </div>
EOT;

  $html_item_unavailable = <<<EOT
    <div class="views-field views-field-created" style="text-align: center">
      <span class="field-content">
        %s
      </span>
    </div>
EOT;

  // Removing not-open hours
  foreach ($vars['start_times'] as $index => &$hour) {

    if ($hour < $data_matrix['daily_settings']['start_time_hm'] || $hour >= $data_matrix['daily_settings']['end_time_hm']) {
      $vars['items'][$hour]['hide'] = true;
    }
  }

  for ($i = 0; $i < (strtotime($end_time) - strtotime($start_time) + 1) / 86400; $i++) {

    foreach ($vars['start_times'] as $hour) {

      $vars['items'][$hour]['hour']  = substr($hour, 0, 5);
      $vars['items'][$hour]['ampm']  = '';
      $vars['items'][$hour]['time']  = $hour;

      $date_time = date ('Y-m-d H:i', strtotime ($start_time) + $i * 86400 + ((int)substr ($hour, 0, 2) * 60*60 + (int)substr ($hour, 3, 5) * 60));

      $data      = @$data_matrix['data'][$i][substr($hour, 0, 5)];

//      echo '<pre>'; print_r($data_matrix); exit;

      if (isset($data['affected_settings'])) {
        if ($data['affected_settings'] -> type == 3) {
          // It's a class
        }
      }

      if ($data)
      {
        $sid = $data['affected_settings'] -> instance_id;

        $args = base64_encode(json_encode(array($vid, $data['affected_settings'] -> nid, $cid, $sid, $date_time, $data['affected_settings']->slot_length)));
      }

      $html_entry = '';
      $html = '';

      $past_time_slot = FALSE;

      if (!isset($data['rowspan'])) {
        $data['rowspan'] = 1;
      }

      if (isset($data['affected_settings']) && isset($data['affected_settings'] -> too_far)) {

        $vars['items'][$hour]['values'][$i]['class']     = 'too-far';

        $html = '
          <div class="views-field views-field-created" style="text-align: center">
            <span class="field-content">
              <span class="too-far-text">' . t('Not yet available to book') . '</span>
            </span>
          </div>
        ';

      }
      elseif (!booking_timeslots_can_book_past_dates() && $date_time < booking_timeslots_get_server_time () && ((!empty($data['type']) && $data['type'] != 'green_box') || (isset($data['affected_settings']) && $data['affected_settings'] -> type == 3))) {

        // Past date slot
        $vars['items'][$hour]['values'][$i]['class']     = 'closed';

        $past_time_slot = TRUE;

      } elseif (!isset($data_matrix['daily_settings']['days'][$i]) || ($hour < $data_matrix['daily_settings']['days'][$i]['start_time_hm'] || $hour >= $data_matrix['daily_settings']['days'][$i]['end_time_hm'])) {
        // Time slot starts before day starting time or after day closing time
        $vars['items'][$hour]['values'][$i]['class']     = 'closed';

      } elseif (isset($data['length']) && booking_timeslots_helper_add_minutes_to_hour_min($hour, $data['length']) > $data_matrix['daily_settings']['days'][$i]['end_time_hm']) {
        // Time slot ends after day closing time
        $vars['items'][$hour]['values'][$i]['class']     = '';

      } else {

        if (isset($data['type']) && $data['type'] == 'green_box' && !is_numeric($cid) && !booking_timeslots_helper_check_duration_available(@$data['affected_settings'], $vars['granularity']))
        {
            $data['rowspan'] = $data['length'] / $grid_granularity;

            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-unavailable calendar-item-availability-unavailable';

            $html = '
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  <span class="unavailable-text">' . t('Unavailable') . '<br /></span>' . t('Try another duration') . '
                </span>
              </div>
            ';
        }
        else
        {
          if (isset($data['type']) && $data['type'] == 'green_box' && $data['length'] < $vars['granularity'])
          {
            $data['rowspan'] = $data['length'] / $grid_granularity;

            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-unavailable calendar-item-availability-unavailable';

            $html = '
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  <span class="unavailable-text">' . t('Unavailable') . '</span>
                </span>
              </div>
            ';

          } elseif (isset($data['type']) && $data['type'] == 'green_box') {

            $vars['items'][$hour]['values'][$i]['class']       = 'calendar-item calendar-item-green-box';

            $vars['items'][$hour]['values'][$i]['attributes']  = 'id="' . str_replace('+', ' ', urlencode(json_encode(array(
              'date'             => $date_time,
              'start_time'       => booking_timeslots_helper_add_minutes_to_hour_min ('00:00', $data['box']['start']),
              'end_time'         => booking_timeslots_helper_add_minutes_to_hour_min ('00:00', $data['box']['end']),
              'grid_granularity' => $grid_granularity,
              'primary_id'       => $vid,
              'secondary_id'     => $data['affected_settings']->nid,
              'tertiary_id'      => $cid,
              'cid'              => $sid,
              'data'             => json_decode ($data['affected_settings'] -> notice),
              'height'           => is_numeric($cid) ? ($data['box']['end'] - $data['box']['start']) : FALSE,
            )))) . '"';

            $data['rowspan'] = ($data['box']['end'] - $data['box']['start']) / $grid_granularity;

           } else {

            $vars['items'][$hour]['values'][$i]['class']       = '';
          }
        }

      }

      if (isset($data['bookings']) || isset($data['type']) && $data['type'] == 'normal') {

        // At least one booking here

        $price            = '$' . $data['price'];
        $duration         = $data['duration'];

        $data['rowspan']  = $duration / $grid_granularity;

        if (($duration == $vars['granularity'] || is_numeric ($cid)) && !$past_time_slot)
        {
          if ($data['capacity'] > 0) {

            $vars['items'][$hour]['values'][$i]['attributes']  = 'id="' . str_replace('+', ' ', urlencode(json_encode(array(
              'date'             => $date_time,
              'start_time'       => substr($hour, 0, 5),
              'end_time'         => booking_timeslots_helper_add_minutes_to_hour_min(substr($hour, 0, 5), $duration),
              'grid_granularity' => $grid_granularity,
              'primary_id'       => $vid,
              'secondary_id'     => $data['affected_settings']->nid,
              'tertiary_id'      => $cid,
              'cid'              => $sid,
              'price'            => $price
            )))) . '"';



            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-available';


            if ($data['capacity'] == $data['capacity_max'])
              $vars['items'][$hour]['values'][$i]['class'] .= ' calendar-item-availability-all';
            else
              $vars['items'][$hour]['values'][$i]['class'] .= ' calendar-item-availability-not-all';


            $html = '
              <div class="tipsy-title">Price: <b>' .  $price. '</b> per <b>' . $duration . '</b>min</div>
              <div class="views-field views-field-title" style="text-align: center">
                <span class="field-content">
                  <a class="book-now" href="' . url('booking/add') . '/' . $args . '">' . t('BOOK NOW!') . '</a>
                </span>
              </div>
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  ' . t('<b>%num</b> slots left', array('%num' => $data['capacity'])) . '<br/ >
                </span>
              </div>
            ';
          } else {

            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-unavailable calendar-item-availability-depleted';

            $html = '
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  <span class="unavailable-text">' . t('Unavailable') . '</span>
                </span>
              </div>
            ';
          }
        } else {

          if ($data['capacity'] > 0) {

            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-unavailable calendar-item-availability-unavailable';

            $html = '
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  <span class="unavailable-text">' . t('<b>!num</b> min', array('!num' => $duration)) . '</span>
                </span>
              </div>
            ';

          } else {

            $vars['items'][$hour]['values'][$i]['class']     = 'calendar-item-unavailable calendar-item-availability-depleted';

            $html = '
              <div class="views-field views-field-created" style="text-align: center">
                <span class="field-content">
                  <span class="unavailable-text">' . t('Unavailable') . '</span>
                </span>
              </div>
            ';

          }

        }

      }

      if (!isset($data['capacity'])) {
        $data['capacity'] = 0;
      }

      if (!isset($data['bookings'])) {
        $data['bookings'] = array();
      }

      if ($data['capacity'] > 0 || $data['bookings']) {

        $vars['items'][$hour]['values'][$i]['records'][] = array (
          'entry'    => $html,
          'title'    => '',
          'rendered' => '',
          'colspan'  => 1,
          'rowspan'  => $data['rowspan'],
          'filled'   => FALSE,
          'avail'    => TRUE,
          'wday'     => $i,
        );
      }

//      $html_entry = sprintf($html_item, sprintf($html_item_available, $price, $minutes, url('booking/add'), $args, t('Book Now'), $description));

      if (booking_timeslots_helper_hour_min_to_minutes($hour) % 60 == (60 - $grid_granularity)) {
        $vars['items'][$hour]['tr-class'] = ' zero-minutes';
        $vars['items'][$hour]['values'][$i]['class'] .= ' zero-minutes';
      }


      for ($r = $grid_granularity; $r < $data['rowspan'] * $grid_granularity; $r += $grid_granularity) {
        $shifted = booking_timeslots_helper_add_minutes_to_hour_min($hour, $r);

        if ($shifted >= $data_matrix['daily_settings']['end_time_hm'])
          continue;

        $vars['items'][$shifted]['values'][$i]['hidden'] = true;
      }

      if (user_access('edit any booking entity')) {

        $booking_entry = '<div class="booking-item"><div class="title">%s</div><a href="%s">Edit booking</a></div>';

        if (!empty($data['bookings'])) {
          foreach ($data['bookings'] as $booking) {

            $duration = $data['duration'];

            $vars['items'][$hour]['values'][$i]['records'][] = array (
              'entry'    => '
                <div class="tipsy-title">Price: <b>' .  $price. '</b> per <b>' . $duration . '</b>min</div>
                <div class="views-field views-field-title booking-item" style="text-align: center">
                  <div class="title">' . t('booked for') . '</div>
                  <span class="field-content">
                    ' . t('<b>%num</b> player(s)', array ('%num' => $booking -> players_original)) . '
                  </span>
                  <div class="link">
                    <a class="" href="' . url('booking/' . $booking -> pid . '/edit') . '">' . t('Edit booking') . '</a>
                  </div>
                </div>
              ',
              'title'    => '',
              'rendered' => '',
              'colspan'  => 1,
              'rowspan'  => 1,
              'filled'   => FALSE,
              'avail'    => TRUE,
              'wday'     => $i,
            );
          }
        }

      }

    }
  }

  ksort($vars['items']);

  if ($view_mode == 'day') {
    $vars['rows']['items'] = &$vars['items'];
  }

}

function booking_timeslots_helper_check_duration_available($settings, $duration) {

  if (!$settings || !$duration)
    return FALSE;

  $data      = json_decode($settings -> notice);
  $price_key = booking_timeslots_get_user_is_member() ? 'members' : 'non_members';

  if ($data -> price -> type == 'regular' && !empty($data -> price -> regular -> $price_key)) {
    return TRUE;
  } elseif ($data -> price -> type == 'non_regular') {
    if (!empty($data -> price -> non_regular -> $duration -> $price_key))
      return TRUE;
  }

  return FALSE;
}

function booking_timeslots_helper_hour_min_to_minutes ($hour_min) {
  return ((int)substr ($hour_min, 0, 2) * 60 + (int)substr ($hour_min, 3, 5));
}

function booking_timeslots_helper_add_minutes_to_hour_min ($hour_min, $minutes) {
  $hour_min_m = ((int)substr ($hour_min, 0, 2) * 60*60 + (int)substr ($hour_min, 3, 5) * 60) + $minutes * 60;

  $date = new DateTime();
  $date -> setTime(0, $hour_min_m / 60);

//  dsm ($hour_min . ' + ' . $minutes . ' become ' . $date -> format ('H:i'));

  return $date->format('H:i');
}

/**
 * Theme preprocess callback
 *
 * Format an calendar month node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_month(&$vars) {
  template_preprocess_booking_timeslots_month_overlap($vars);
}

/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_month_overlap(&$vars) {

  $month = $vars['view'] -> argument['date_argument'] -> argument;

  list($time_start, $time_end) = booking_timeslots_helper_calculate_month_view_range($month);
  //dsm($time_start, $time_end);
  template_preprocess_booking_timeslots_general($vars, 'month', $time_start, $time_end);
}

function template_preprocess_booking_timeslots_opening_hours_admin(&$vars) {

  $node = node_load(arg(1));

  $vars['node'] = $node;

}
