<?php
/**
 * @file
 * Template for the opening hours admin interface.
 *
 * Is not really a template in Drupal-sense, mainly a container for the
 * markup necessary to render the opening hours interface.
 */

  drupal_add_css(drupal_get_path('module', 'booking_timeslots') .'/templates/booking_timeslots.css');
  drupal_add_js(array('booking_timeslots_calendar_granularity' => booking_timeslots_get_calendar_granularity()), 'setting');

  $configuration = booking_timeslots_get_configuration();


?>
<div id="opening-hours-admin">
  <p class="placeholder"><?php print t('Loading administration interface…'); ?></p>
</div>

<script type="text/template" id="oho-admin-main-template">
  <ul class="navigation clear-block">
    <li><a class="prev-week" href="#prev" title="<?php print t('Previous week'); ?>">‹</a>
    <li><a class="current-week" href="#current"><?php print t('Current week'); ?></a>
    <li><a class="next-week" href="#next" title="<?php print t('Next week'); ?>">›</a>
  </ul>

  <div class="dateheader">
    <h2><?php echo t('Week !week, !year', array(
      '!week' => '<%- weekNumber %>',
      '!year' => '<%- year %>',
    )); ?></h2>
    <h3>
      <span class="date from"><%- fromDate %></span>
      <% if (toDate) { %>
        – <span class="date to"><%- toDate %></span>
      <% } %>
    </h3>
  </div>

  <table class="days">
    <thead>
      <tr>
        <% _.each(dateHeaders, function (header) { %><th><%= header %></th><% }); %>
      </tr>
    </thead>
    <tbody><tr></tr></tbody>
  </table>
</script>

<script type="text/template" id="oho-instance-display-template">
  <span class="start_time"><%= start_time %></span> –
  <span class="end_time"><%= end_time %></span>
</script>

<script type="text/javascript">
  jQuery(function () {
    Drupal.OpeningHours.InstanceEditView.prototype.saveButtonBackup =
    Drupal.OpeningHours.InstanceEditView.prototype.saveButton;

    Drupal.OpeningHours.InstanceEditView.prototype.saveButton = function () {

      var priceMatrix = {}

      jQuery('tr.oho-extra-price-rows').each (function () {


        var dataTime            = jQuery(this).find ('input.time').val ();
        var dataPriceNonMembers = jQuery(this).find ('input.non_members').val ();
        var dataPriceMembers    = jQuery(this).find ('input.members').val ();

        priceMatrix [dataTime] = {
          non_members: dataPriceNonMembers,
          members:     dataPriceMembers
        }

      });

      jQuery('input#oho-notice').val(JSON.stringify({
        secondary_id: 0,

        price: {
          type:      jQuery('#oho-non-regular-price-tick:checked').length ? 'non_regular' : 'regular',
          regular: {
            non_members: jQuery('input#oho-extra-price-non-members').val(),
            members:     jQuery('input#oho-extra-price-members').val(),
          },
          non_regular: priceMatrix
        },

        max_time_ahead_rsrv_weeks: jQuery('select#oho-extra-max-time-ahead-rsrv-weeks').val(),
        max_time_ahead_rsrv_days: jQuery('select#oho-extra-max-time-ahead-rsrv-days').val(),
        max_time_ahead_rsrv_hours: jQuery('select#oho-extra-max-time-ahead-rsrv-hours').val(),

        min_time_ahead_rsrv_weeks: jQuery('select#oho-extra-min-time-ahead-rsrv-weeks').val(),
        min_time_ahead_rsrv_days: jQuery('select#oho-extra-min-time-ahead-rsrv-days').val(),
        min_time_ahead_rsrv_hours: jQuery('select#oho-extra-min-time-ahead-rsrv-hours').val()

      }));

    this.model._previousAttributes = {a:1};

    var slotLength = jQuery('input#oho-extra-length').val();

    <?php if($node -> type == $configuration['ct_name_3']): ?>

      var startHour = parseFloat (this.model.attributes.start_time.substr (0, 2));
      var startMin  = parseFloat (this.model.attributes.start_time.substr (3, 6));
      var start     = startHour * 60 + startMin;
      var endHour   = parseFloat (this.model.attributes.end_time.substr (0, 2));
      var endMin    = parseFloat (this.model.attributes.end_time.substr (3, 6));
      var end       = endHour * 60 + endMin;

      slotLength = end - start;

    <?php endif; ?>



	  this.model.attributes.slot_length = slotLength;
	  this.model.attributes.capacity = jQuery('input#oho-extra-capacity').val();

      Drupal.OpeningHours.InstanceEditView.prototype.saveButtonBackup.apply(this, arguments);
    };
  });

  NonRegularPriceTickChanged = function (skipEvent) {

    var checked = jQuery('#oho-non-regular-price-tick:checked').length;

    if (checked) {
      jQuery('#oho-regular-price-container input').attr ('disabled', true);
      jQuery('#oho-non-regular-price-container').fadeIn ();

      if (!skipEvent)
      {
        // Populating non regular prices by regular prices

        jQuery('#oho-non-regular-price-tbody input.members').val (jQuery('input#oho-extra-price-members').val ());
        jQuery('#oho-non-regular-price-tbody input.non_members').val (jQuery('input#oho-extra-price-non-members').val ());
      }
    }
    else
    {
      jQuery('#oho-regular-price-container input').removeAttr ('disabled');
      jQuery('#oho-non-regular-price-container').fadeOut ();
    }

  }

  NonRegularPriceAddRow = function () {
    jQuery('#oho-non-regular-price-tbody').append ('<tr><td><span class="time-start">08:00</span> - <span class="time-end">09:00</span></td><td><input type="text" value="0" /></td><td><input type="text" value="0" /></td></tr>');
  }

</script>
<script type="text/template" id="oho-instance-edit-template">
  <div class="form-item form-type-textfield form-item-title views-exposed-form form">

    <%
      setTimeout("jQuery('div.ui-dialog-content').height(500); jQuery('div.ui-dialog').css('top', '100px')", 1);
    %>

    <form action="." id="oho-instance-edit-form" class="node-form node-venue-form">
      <fieldset class="date-time repeat">

                <label><?php print t('Opening hours'); ?></label>
                <input type="text" class="date text" size="9" title="<?php print t('Date'); ?>" value="<%= date %>" <% if (!isNew) { %>disabled="disabled"<% } %> />
                <input type="text" class="start_time text" size="7" title="<?php print t('Start time'); ?>" value="<%= start_time %>" />
                to
                <input type="text" class="end_time text" size="7" title="<?php print t('End time'); ?>" value="<%= end_time %>" />

                        <label for="oho-repeat-rule"><?php print t('Repeat'); ?></label>
                        <select name="oho-repeat-rule" id="oho-repeat-rule">
                          <option value="">never</option>
                          <option value="weekly">every week</option>
                        </select>
                        <label class="end" for="oho-repeat-end-date"><?php print t('until'); ?></label>
                        <input type="text" class="text end repeat-end-date" name="oho-repeat-end-date" id="oho-repeat-end-date" size="9" title="<?php print t('End date'); ?>" value="<%= repeat_end_date %>" />

      </fieldset>

      <%

        if (this.model.attributes.notice [0] != '{')
          this.model.attributes.notice = '{}';

        var data = JSON.parse (this.model.attributes.notice);

        if (!data.price)
          data.price = {regular: {non_members: '', members: ''}};

        if (!data.price.type)
          data.price.type = 'non_regular';

        if (!data.price.regular)
          data.price.regular = {non_members: 0, members: 0};

        if (!data.slot_length)
          data.slot_length = 60;

        if (!data.capacity)
          data.capacity = 1;

        if (!this.model.attributes.capacity)
          this.model.attributes.capacity = 1;

        if (data.max_time_ahead_rsrv_weeks == undefined)
          data.max_time_ahead_rsrv_weeks = 0;

        if (data.max_time_ahead_rsrv_days == undefined)
          data.max_time_ahead_rsrv_days = 0;

        if (data.max_time_ahead_rsrv_hours == undefined)
          data.max_time_ahead_rsrv_hours = 0;

        if (data.min_time_ahead_rsrv_weeks == undefined)
          data.min_time_ahead_rsrv_weeks = 0;

        if (data.min_time_ahead_rsrv_days == undefined)
          data.min_time_ahead_rsrv_days = 0;

        if (data.min_time_ahead_rsrv_hours == undefined)
          data.min_time_ahead_rsrv_hours = 1;

      %>

      <?php if($node -> type != $configuration['ct_name_1']): ?>

      <fieldset class="details">
        <label><?php print t('Price'); ?></label>

        <div id="oho-regular-price-container">

          <table>
            <thead>
              <tr>
                <th>Pricing for members</th>
                <th>Pricing for non-members</th>
              </tr>
            </thead>
            <tbody id="oho-non-regular-price-tbody">
              <tr>
                <td><input type="text" class="member" id="oho-extra-price-members" value="<%= data.price.regular.members %>" /></td>
                <td><input type="text" class="non_member" id="oho-extra-price-non-members" value="<%= data.price.regular.non_members %>" /></td>
              </tr>
            </tbody>
          </table>

        </div>

        <?php if($node -> type != $configuration['ct_name_3']): ?>

        <br />

        <label><?php print t('Non regular price'); ?>

          <%  if (data.price.type == "regular") { %>
            <input type="checkbox" onchange="NonRegularPriceTickChanged()" id="oho-non-regular-price-tick" />
          <% } else { %>
            <input type="checkbox" checked onchange="NonRegularPriceTickChanged()" id="oho-non-regular-price-tick" />
          <% } %>

        </label>

        <div id="oho-non-regular-price-container">

          <table>
            <thead>
              <tr>
                <th>Duration</th>
                <th>Pricing for members</th>
                <th>Pricing for non-members</th>
              </tr>
            </thead>
            <tbody id="oho-non-regular-price-tbody">
              <%
                // Generating hour record

                data.slot_length = parseInt (data.slot_length);

                if (data.slot_length < 1)
                  data.slot_length = 1;

                var records = [];

                var startHour = parseFloat (this.model.attributes.start_time.substr (0, 2));
                var startMin  = parseFloat (this.model.attributes.start_time.substr (3, 6));
                var start     = startHour * 60 + startMin;
                var endHour   = parseFloat (this.model.attributes.end_time.substr (0, 2));
                var endMin    = parseFloat (this.model.attributes.end_time.substr (3, 6));
                var end       = endHour * 60 + endMin;

                var timeRanges = <?php echo json_encode(booking_timeslots_get_durations()); ?>;

                if (!data.price.non_regular)
                  data.price.non_regular = {};

                for (var i in timeRanges)
                {
                  var name = timeRanges [i];

              %>

                 <tr class="oho-extra-price-rows">
                  <td><input type="text" class="time" value="<%= i %>" style="display: none" /><span class="time-start"><%= name %></span></td>
                  <td><input type="text" class="members" value="<%= !data.price.non_regular[i] ? "" : data.price.non_regular[i]['members'] %>" /></td>
                  <td><input type="text" class="non_members" value="<%= !data.price.non_regular[i] ? "" : data.price.non_regular[i]['non_members'] %>" /></td>
                </tr>

              <% } %>

            </tbody>
          </table>
        </div>

      <?php endif; ?>

      </fieldset>

      <?php if($node -> type != $configuration['ct_name_3']): ?>

      <fieldset class="details">
        <table class="simple">
          <tbody>
            <tr>
              <td style="display: none">
                <label for="oho-extra-length"><?php print t('Slot length'); ?></label>
                <input type="text" class="text" name="oho-length" id="oho-extra-length" title="" size="8" value="<%= data.slot_length %>" />
              </td>
              <td>
                <label for="oho-extra-capacity"><?php print t('Capacity'); ?></label>
                <input type="text" class="text" name="oho-capacity" id="oho-extra-capacity" title="" size="8" value="<%= this.model.attributes.capacity %>" />
              </td>
            </tr>
          </tbody>
        </table>
      </fieldset>


      <?php endif; ?>

      <fieldset>
        <label><?php print t('How long prior to a reservation can people book?'); ?></label>
        <span style="font-size:12px"><?php print t('This option will ensure that users can book <b>specified time</b> before the reservation at the latest.'); ?></span>
        <table class="simple">
          <thead>
            <tr>
              <th style="display: none">
                <?php print t('Weeks'); ?>
              </th>
              <th style="display: none">
                <?php print t('Days'); ?>
              </th>
              <th style="display: none">
                <?php print t('Hours'); ?>
              </th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                <label for="oho-extra-min-time-ahead-rsrv-weeks"><?php print t('Weeks'); ?></label>
                <select name="oho-min-time-ahead-rsrv-weeks" id="oho-extra-min-time-ahead-rsrv-weeks" title="">
                  <% for (var i = 0; i <= 53; i++) { %>
                    <option value="<%= i %>" <% if (data.min_time_ahead_rsrv_weeks == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
              <td>
                <label for="oho-extra-min-time-ahead-rsrv-days"><?php print t('Days'); ?></label>
                <select name="oho-min-time-ahead-rsrv-days" id="oho-extra-min-time-ahead-rsrv-days" title="">
                  <% for (var i = 0; i <= 6; i++) { %>
                    <option value="<%= i %>" <% if (data.min_time_ahead_rsrv_days == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
              <td>
                <label for="oho-extra-min-time-ahead-rsrv-hours"><?php print t('Hours'); ?></label>
                <select name="oho-min-time-ahead-rsrv-hours" id="oho-extra-min-time-ahead-rsrv-hours" title="">
                  <% for (var i = 0; i <= 23; i++) { %>
                    <option value="<%= i %>" <% if (data.min_time_ahead_rsrv_hours == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
            </tr>
          </tbody>
        </table>
      </fieldset>

      <fieldset>
        <label><?php print t('How far in advance can people book? (Leave zeroes to disable this option).'); ?></label>
        <span style="font-size:12px"><?php print t('This option will disable dates that are beyond the <b>specified time</b>.'); ?></span>
        <table class="simple">
          <thead>
            <tr>
              <th style="display: none">
                <?php print t('Weeks'); ?>
              </th>
              <th style="display: none">
                <?php print t('Dats'); ?>
              </th>
              <th style="display: none">
                <?php print t('Hours'); ?>
              </th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                <label for="oho-extra-max-time-ahead-rsrv-weeks"><?php print t('Weeks'); ?></label>
                <select name="oho-max-time-ahead-rsrv-weeks" id="oho-extra-max-time-ahead-rsrv-weeks" title="">
                  <% for (var i = 0; i <= 53; i++) { %>
                    <option value="<%= i %>" <% if (data.max_time_ahead_rsrv_weeks == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
              <td>
                <label for="oho-extra-max-time-ahead-rsrv-days"><?php print t('Days'); ?></label>
                <select name="oho-max-time-ahead-rsrv-days" id="oho-extra-max-time-ahead-rsrv-days" title="">
                  <% for (var i = 0; i <= 6; i++) { %>
                    <option value="<%= i %>" <% if (data.max_time_ahead_rsrv_days == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
              <td>
                <label for="oho-extra-max-time-ahead-rsrv-hours"><?php print t('Hours'); ?></label>
                <select name="oho-max-time-ahead-rsrv-hours" id="oho-extra-max-time-ahead-rsrv-hours" title="">
                  <% for (var i = 0; i <= 23; i++) { %>
                    <option value="<%= i %>" <% if (data.max_time_ahead_rsrv_hours == i) { %> selected <% } %>><%= i %></option>
                  <% } %>
                </select>
              </td>
            </tr>
          </tbody>
        </table>
      </fieldset>


      <?php endif; ?>

      <fieldset class="details" style="display: none">
        <label for="oho-notice"><?php print t('Notice'); ?></label>
        <input type="text" class="notice text" name="oho-notice" id="oho-notice" title="<?php print t('Whatâ€™s special about this instance?'); ?>" size="60" value="<%= notice %>" />
      </fieldset>

    </form>



    <script type="text/javascript">
      jQuery(function () {
        setTimeout ("NonRegularPriceTickChanged(true)", 1);

        setTimeout (function () {

          jQuery('input.start_time, input.end_time').timeEntry('destroy');

          jQuery('.start_time, .end_time').timeEntry({
            show24Hours: true,
            spinnerImage: false,
            timeSteps: [1, Drupal.settings.booking_timeslots_calendar_granularity, 1]
          });
        }, 10);


      });
    </script>

  </div>
</script>
