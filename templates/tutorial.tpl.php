<ol>
  <li>Content types</li>
  <p>
    <b>Booking Timeslots</b> module requires at least two content types to function properly: Primary and Secondary.<br />
    
    <ul>
      <li>Primary content type</li>
      <p>
        Primary content type is used to define allowed opening hours for the related Secondary content type nodes.

        <div class="installation">
          <h2>How to create</h2>
          <p>
            <ol>
              <li>Create a new content type with name e.g. "Venue".</li>
              <li>Click on the "<b>Opening hours</b>" in the left accordion and select "<b>Enable opening hours for this content type</b>".</li>
            </ol>
            
            <p>
              That's all. Primary content type doesn't need any fields. You may now select that content type in module's settings screen (<b>Tertiary content type</b>).
            </p>
          </p>
        </div>
        
        <div class="installation">
          <h2>Opening hours</h2>
          <p>
            To edit node's opening hours, click on the "<b>Opening hours</b>" tab on the node's view/edit screen.
            
            <div><h2>After clicking on the day box you will see following form:</h2></div>
            
            <p>
              <img src="/<?php print $dir; ?>/images/tutorial_opening_hours_venue.png" style="border: 0;" />
            </p>
            
            <p>
              In primary content type opening hours screen you cannot set prices and other information as primary content type purpose is to set allowed opening hours for secondary content type only.
            </p>
            
          </p>
        </div>
        
        <div class="clear-both"></div>
      </p>
      
      <div class="clear-both"></div>

      <li>Secondary content type</li>
      <p>
        Secondary content type may be used in the way that we define bookable time ranges in it's opening hours tab or as the Tertiary content type filter (Only when Tertiary content type has been enabled).
        
        <div class="installation">
          <h2>How to create</h2>
          <p>
            Secondary content type requires a field to be created which is a reference to primary content type.
            
            <div><h2>Required fields:</h2></div>

            <div class="table">
              <table>
                <thead>
                  <tr>
                    <th></th>
                    <th>Field purpose</th>
                    <th>Field Type</th>
                    <th>Widget</th>
                    <th>Required</th>
                  </tr>
                </thead>
                <tbody>
                  <tr>
                    <td>#1</td>
                    <td>Reference to <b>primary</b> content type</td>
                    <td>Entity Reference</td>
                    <td>Select list</td>
                    <td><b>Yes</b></td>
                  </tr>
                </tbody>
              </table>
            </div>

            <div><h2>Step by step:</h2></div>
            
            <ol>
              <li>Create a new content type with name e.g. "Facility".</li>
              <li>Click on the "<b>Opening hours</b>" in the left accordion and select "<b>Enable opening hours for this content type</b>".</li>
              <li>Click "<b>Save and add fields</b>".</li>
              <li>Add a new field (#1) which would be a reference to primary content type nodes.</li>
            </ol>
            
            <p>
              You may now select that content type in module's settings screen (<b>Secondary content type</b>).
            </p>
            
          </p>
        </div>
        
        <div class="installation">
          <h2>Opening hours</h2>
          <p>
            To edit node's opening hours, click on the "<b>Opening hours</b>" tab on the node's view/edit screen.
            
            <div><h2>After clicking on the day box you will see following form:</h2></div>
            
            <p>
              <img src="/<?php print $dir; ?>/images/tutorial_opening_hours_non_venue.png" style="border: 0;" />
            </p>
            
            <p>
              There you can set prices, capacity (how many people may fit) and other options.
            </p>
          </p>
        </div>
        

        </p>
      
      <div class="clear-both"></div>
      
      <li>Tertiary content type</li>
      <p>
        It is a specialized content type and allows booking only the whole time ranges defined in the opening hours tab. In other words, you cannot book 15min of the time range but whole time range at once, e.g.: from 10am to 4pm.
        
        <div class="installation">
          <h2>How to create</h2>
          <p>
            Tertiary content type requires three field to be created, references to primary and secondary content type nodes and third field, a capacity indicator. It's convenient to firstly create the quaternary content type if you'd like to use quaternary content type because in this step you would be prompted to optionally choose field acting as a reference to quaternary content type.
            
            <div><h2>Required fields:</h2></div>
            
            <div class="table">
              <table>
                <thead>
                  <tr>
                    <th></th>
                    <th>Field purpose</th>
                    <th>Field Type</th>
                    <th>Widget</th>
                    <th>Required</th>
                    <th>Details</th>
                  </tr>
                </thead>
                <tbody>
                  <tr>
                    <td>#1</td>
                    <td>Reference to <b>secondary</b> content type</td>
                    <td>Entity Reference</td>
                    <td>Select list</td>
                    <td><b>Yes</b></td>
                    <td></td>
                  </tr>
                  <tr>
                    <td>#2</td>
                    <td>Reference to <b>quaternary</b> content type</td>
                    <td>Entity Reference</td>
                    <td>Select list</td>
                    <td><b>Yes</b></td>
                    <td></td>
                  </tr>
                  <tr>
                    <td>#3</td>
                    <td>Capacity indicator</td>
                    <td>Integer</td>
                    <td>Text field</td>
                    <td><b>Yes</b></td>
                    <td>Use "1" for <b>Minimum</b> and <b>Default value</b> fields</td>
                  </tr>
                </tbody>
              </table>
            </div>

            <div><h2>Step by step:</h2></div>
            
            <ol>
              <li>Create a new content type with name e.g. "Class".</li>
              <li>Click on the "<b>Opening hours</b>" in the left accordion and select "<b>Enable opening hours for this content type</b>".</li>
              <li>Click "<b>Save and add fields</b>".</li>
              <li>Add a new field (#1) which would be a reference to secondary content type nodes.</li>
              <li>Optionally add a new field (#2) which would be a reference to quaternary content type nodes.</li>
              <li>Add a new field (#3) which would act as the capacity indicator.</li>
            </ol>
            
            <p>
              You may now select that content type in module's settings screen (<b>Tertiary content type</b>).
            </p>
            
          </p>
        </div>

        <div class="installation">
          <h2>Opening hours</h2>
          <p>
            To edit node's opening hours, click on the "<b>Opening hours</b>" tab on the node's view/edit screen.
            
            <div><h2>After clicking on the day box you will see following form:</h2></div>
            
            <img src="/<?php print $dir; ?>/images/tutorial_opening_hours_class.png" style="border: 0;" />
          </p>
        </div>

        <div class="clear-both"></div>
      </p>
      
      <div class="clear-both"></div>
      
      <li>Quaternary content type</li>
      <p>
        Used to filter tertiary content type. Doesn't utilize opening hours and have no options.

        <div class="installation">
          <h2>How to create</h2>
          <p>
            <ol>
              <li>Create a new content type with name e.g. "Instructor".</li>
            </ol>
            
            <p>
              That's all. Quaternary content type doesn't need any fields and doesn't utilize <b>opening hours</b>. You may now select that content type in module's settings screen (<b>Quaternary content type</b>).
            </p>
          </p>
        </div>

      </p>

      <li id="views-integration">Views integration</li>
      <p>
        Currently module supports displaying schedule only on the <u>Primary Content Type</u> pages, so please just select content type used as the primary one.
      </p>
      <p>
        There is a <u>built-in</u> calendar view <b>Bookings Schedule</b> (bt_schedule) already in the module's features which is used to display tabs on the content type's pages. Just make sure the view is enabled and if there is no such view - upgrade the module and clear the cache.
      </p>

    </ul>
    
  </p>
  
  <li>Categories</li>
  <p>
    You may enable additional filter to categorize secondary content type using exising term reference field.

    <div class="installation">
      <h2>Requirements</h2>
      <p>
        <ol>
          <li>Categories filter requires a field of type <b>Term reference</b> in the secondary content type, if you didn't yet create the field, create it using the following description:</li>
        </ol>
    
        <div class="table">
          <table>
            <thead>
              <tr>
                <th></th>
                <th>Field purpose</th>
                <th>Field Type</th>
                <th>Widget</th>
                <th>Required</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td>#1</td>
                <td>Category</td>
                <td>Term reference</td>
                <td>Any</td>
                <td>No</td>
              </tr>
            </tbody>
          </table>
        </div>
    
        <p>
          Now select that field in the module settings screen (<b>Category field</b>)
        </p>
      </p>
    </div>

  </p>

  <li>Calendar view</li>
  <p>
    As noted in <a href="#views-integration">views integration</a> point, there is a <u>built-in</u> calendar view <b>Bookings Schedule</b> (bt_schedule) already in the module's features which is used to display tabs on the content type's pages. Just make sure the view is enabled and if there is no such view - upgrade the module and clear the cache.
  </p>
  <p>
  	Note the opening hours for Primary Content Type <b>restricts opening hours</b> of Secondary Content Type. That mean if a Venue (Primary Content Type) is open from 8 AM to 6 PM then Court (Secondary Content Type) cannot be open before 8 AM or closed after 6 PM.
  </p>
  <p>
  	After proper setup, you should be able to see something more like this:<br/><br/>
	<img src="/<?php print $dir; ?>/images/tutorial_how_it_should_look.png" style="border: 0;" />
  </p>

  <li>Managing bookings</li>
  <p>
    <i>TODO</i>
  </p>
  
</ol>
