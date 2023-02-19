
# Version 4.6 Release Notes

  Version 4.6 fixes local timezone difficulties that were not fixed as
  advertised in version 4.5 for FlowGrapher and FlowViewer. Thanks goes
  to Randy Feeney. Also note that version 4.6 removes the "$time_zone"
  configurable parameter from FlowViewer_Configuration. Timezone is now
  exclusively extracted from the system, using the 'date' function. This
  version fixes a problem with FlowGrapher not correctly displaying the
  smallest flows when requested (e.g., Detail Lines: -100 for smallest
  100 flows.) Fixes improper listing of very old Saved files.

# Version 4.5 Release Notes

  Version 4.5 resolves an unfortunate name clash in commercial space and
  renames FlowTracker to FlowMonitor. The situation where SiLK data is
  saved in UTC (GMT) time, but the system is left in local time has been
  fixed (thanks to Kees Leune.) A new configuration variable
  "$silk_compiled_localtime" has been added for the environment where SiLK
  has been comipled with the --enable-localtime switch. FlowGrapher_Analyze
  has been fixed to handle hyper-links to IPv6 hosts properly. SiLK IPsets
  can now be input through the various tool menus. A problem with
  multi-word Dashboards and Group creation has been fixed. Corrected
  flows/second initiated calculation. Added the ability to bypass the
  printing of pulldowns on the bottom service bar. Fixed an error with 
  filtering on port equal to '0'. Fixed 'Len' field output for some
  FlowGrapher reports. New parameter: $ipfix_default_device allows IPFIX
  users to pre-select a primary device (e.g., using one sensor only.)
  Extended pie-charts to some Printed reports. A new parameter
  $site_config_file is added to make it easier to accomodate various
  SiLK stored data file structures.

  New FlowViewer_Configuration.pm parameters in v4.5:

  $silk_compiled_localtime   - "Y" if SiLK compiled with local timezone
  $ipset_directory           - Directory where IPsets can be found
  $use_bottom_pulldowns      - Will exclude pulldowns on bottom of UI
  $ipfix_default_device      - Controls the default in device_name pulldown
  $sensor_config_file        - Changed from $sensor_config_directory
  $site_config_file          - Left blank (= "";) will look in rootdir

  Note: the rename of FlowTracker to FlowMonitor includes default names
  for FlowMonitor related directories. The defaults that will previal if
  no changes are made are:

  $monitor_directory       = "/var/www/html/FlowMonitor";
  $monitor_short           = "/FlowMonitor";
  $filter_directory        = " ... /FlowMonitor_Files/FlowMonitor_Filters";  
  $rrdtool_directory       = " ... /FlowMonitor_Files/FlowMonitor_RRDtool";

  For users who are upgrading, these can be revised back to 'FlowTracker'
  (or whatever) with no problem. The alternative is to simply rename the
  existing directories.

# Version 4.4 Release Notes

  Version 4.4 introduces two new significant capabilities; multiple
  dashboards and FlowGrapher Analysis. The user can now set up 
  more than one active dashboard with links on every page to get
  to each dashboard. Uses include multiple networks, data centers,
  or multiple users. The new version also permits users to quickly 
  de-aggregate FlowGraphs into the largest (3 to 10) source or
  destination IP or Port contributors. This is particularly useful in
  analyzing peaks in graphs based on "Flows" for DDOS and the like.
  The multiple dashboard changes touched most of the scripts. There
  is one new script, FlowGrapher_Analyze, and the script
  FlowMonitor_AltDashboard is deprecated. The code was cleansed of
  confusing 'prorated (_P)' distinctions. Version 4.4 also greatly
  improves the user's ability to interface with various SiLK
  configurations other than the 'flow-tools-like' one suggested in
  earlier documentation. Note: SiLK users must upgrade SiLK to 
  version 3.8.0 or newer.

# Version 4.3 Release Notes

  Version 4.3 introduces a new FlowViewer report called "Detect
  Scanning" which employs scanning detection software in both
  flow-tools (flow-dscan) and SiLK (rwscan.) The output from each 
  report includes links to the individual scanner sources detected.
  The link will create a FlowGrapher report for that host. Modified
  field checks to permit 32 bit AS entries. However, this is for the
  future when SiLK begins to handle the fields. Thanks Veasna Long.

# Version 4.2.2 Release Notes

  Version 4.2.2 is a quick emergency fix with limited changes. An
  array (@temp_ports) in FlowViewer_Utilities was not initialized
  and was acumulating contents over many FlowMonitor_Collector runs.
  This was causing the whole FlowMonitor_Collector run to slow down
  gradually over time as this array would have to get sorted for each
  SiLK FlowMonitor (this caused no problems for flow-tools only 
  users.)

# Version 4.2.1 Release Notes

  Version 4.2.1 extends the use of the recently discovered flow-report
  option "linear-interpolated-flows-octets-packets" to FlowMonitor 
  processing (see User's Guide for further discussion.) This provides 
  significant speed-ups for FlowMonitor_Recreate (above 75%) and 
  FlowMonitor_Collector (which will now permit many more FlowMonitors.)
  The FlowMonitor_Recreate processing for SiLK was remarkably poorly
  implemented and this has been corrected introducing unspeakable
  performance gains there as well. The same use of SiLK prefiltering
  that was introduced for FlowMonitor in v4.2 has been applied to
  FlowGrapher for a speed-up there. A new capability is added to work
  with different international date formats. The distribution includes a
  new capability, FlowViewer_CleanSiLK, to monitor and adjust diskspace
  used by IPFIX devices addressing the SiLK deficiency of not having the
  valuable flow-capture feature (-E) of active diskspace usage control.
  Made the use of prefiltered files or CONCATs non-optional and fixed
  SiLK processing of flows and packets for FlowMonitor_Collector. Modest
  changes have been made to FlowViewer_CleanFiles, FV.cgi and
  flowmonitor_grapher_nonlazy. An error causing packets to be monitored as
  flows in some cases was fixed. A new tool, flowmonitor_grapher_recent
  will re-graph (nonlazy) only recently created FlowMonitors.

# Version 4.2 Release Notes

  Version 4.2 incorporates the equivalent of "use existing concatenations"
  for SiLK based FlowMonitors. FlowViewer takes advantage of previously
  concatenated flow-tools files during FlowMonitor_Collector processing
  to speed up the whole run. Now it does this for SiLK files as well by
  performing rwfilter "INPUT" filtering only once for all FlowMonitors
  that are based on the same sensor/class combination. This is a pretty
  significant speed-up and will permit the user to have many more 
  FlowMonitors. For example, our implementation, a combination of 
  flow-tools and SiLK based data, now processes 250 FlowMonitors in 35
  seconds (prior to the new version this was taking 50 seconds.) The new
  version also corrects processing of exporters [M. Donnelly]. Excluded
  fields (e.g., protocols=-17) were being accepted for SiLK FlowMonitors
  despite SiLK not being able to handle them; this was fixed. The 
  analyze_netflow_packets tool has been fixed for IPv6 addresses. Four new
  date conversion utilities have been added to the 'tools' subdirectory.
  FlowMonitor_Collector was modified in version 4.0 to be able to create
  FlowMonitors for flows and packets (as well as bits.) This was
  inadvertantly dropped in version 4.1 and is restored in this update 
  to version 4.2 [7/31].

# Version 4.1 Release Notes

  Version 4.1 includes a new FlowGrapher capability that creates reports
  3 to 4 times faster than previously. The detail lines are a little bit 
  different. The previous capability is retained offering the user a choice
  from the input form interface. The new FlowGrapher report type is 
  "Aggregated". It makes use of a heretofore missed flow-tools capability
  known as the flow-report "linear-interpolated-flows-octets-packets" option
  which aggregates flows, octets, or packets into time buckets. This moves
  that processing into the compiled "C" code of flow-tools. New FlowMonitor
  capabilities are added to monitor flows or packets as well as the previously
  available octets. Version 4.1 introduces the ability to maintain different
  dashboards for different users (please see the User's Guide for how to do
  this. The new version includes a new FV_Relay.cgi script. The new version
  fixes a flaw in FlowMonitor_Collector that erroneously monitored protocols,
  tcp_flags, and tos_fields when using SiLK (thanks C. Spitzlay.) It also
  includes some small fixes like making directory creation a little easier,
  fixes removing (and adding) Trackings from the Dashboard and removes some
  minor extraneous formatting. Fixed problem with FlowMonitor_Collector
  processing of SiLK interface filtering [07/09/13]. Fixed initialization
  of @ipfix_devices in FlowViewer_Configuration.pm [Thanks M. Donnelly.]

# Version 4.0 Release Notes

  Version 4.0 is a major upgrade that enables FlowViewer to handle IPFIX 
  netflow data (i.e., v9, etc.) The User Interface has been completely redone
  and now features a Dashboard. Aside from the new collector interface and user
  interface, version 4.0 introduces some new capabilities:
 
   1. FlowViewer report sorting by column header
   2. Dashboard of thumbnail versions of selected FlowMonitor graphs
   3. Ability to 'recreate' FlowMonitors, starting at a time specified in the 
      past

  The distribution manifest has changed significantly.

##  Preserved Scripts, Files, and Tools:

   FlowViewer.cgi                Modified for new user interface.
   FlowViewer_Main.cgi           Modified for new user interface.
   FlowViewer_Relay.cgi          No change.
   FlowViewer_Save.cgi           Significant modification.
   FlowGrapher.cgi               Modified for new user interface.
   FlowGrapher_Main.cgi          Modified for new interface.
   FlowGrapher_Colors            No change.
   FlowGrapher_Relay.cgi         No change.
   FlowGrapher_Sort.cgi          Significant modification.
   FlowMonitor.cgi               Modified for new user interface.
   FlowMonitor_Collector         Modified to process stored SiLK data.
   FlowMonitor_Grapher           Modified to update Thumbnails.
   FlowMonitor_Group             Modified for new user interface.
   FlowMonitor_Dumper            Modified for new user interface.
   FlowMonitor_Relay.cgi         No change.
   FlowViewer_CleanASCache       No change.
   FlowViewer_CleanFiles         Minor changes.
   FlowViewer_CleanHostCache     No change.
   FlowViewer_Configuration.pm   Modifications for SiLK and user interface.
   FlowViewer_Utilities.pm       Removed filter output processing.
   NamedInterfaces_Devices       No change.
   NamedInterfaces_Exporters     No change.
   flowcapture_restart           No change.
   flow-capture-table.conf       No change.
   flowmonitor_restart           No change.
   performance_check             Parse FlowMonitor logs and report performance
   rsync_flows                   Rsync all of raw flow data to backup host
   rsync_monitors               Rsync all of Tracking data to backup host

##  New Scripts, Files, and Tools

   FlowViewer_Replay.cgi         Presents saved FlowViewer reports
   FlowViewer_SaveManage.cgi     Manages saved reports
   FlowViewer_Sort.cgi           Sorts FlowViewer reports
   FlowViewer_UI.cgi             Utilities for creating user interface
   FlowGrapher_Replay.cgi        Presents saved FlowGrapher reports
   FlowMonitor_Dashboard.cgi     Manages the Dashboard contents
   FlowMonitor_Display.cgi       Presents a FlowMonitor
   FlowMonitor_DisplayPublic.cgi Presents a FlowMonitor from Public list
   FlowMonitor_Management.cgi    Manages FlowMonitors (e.g., remove, etc.)
   FlowMonitor_Recreate          Background process to recreate FlowMonitors
   FlowMonitor_Thumbnail         Invoked to create a Thumbnail FlowMonitor
   FlowViewer.css                FlowViewer cascading style sheet
   FV_button.png                 New button link to FlowViewer from front page
   FG_button.png                 New button link to FlowGrapher from front page
   FM_button.png                 New button link to FlowMonitor from front page
   convert_pre40_filters         Converts old saved filters (pre version 4.0).
   flowmonitor_archive_restore   Restores archived FlowMonitors gone astray
   flowmonitor_grapher_nonlazy   Forces a re-graphing of all FlowMonitor graphs
   resize_rrdtools               Extends RRDtools created prior to 3-Year graph
   rwflowpack_start              One-line script starts SiLK collector
   analyze_netflow_packets       Script analyzes TCPDUMP captured netflow data 

##  Removed Scripts and Files

   FlowViewer_SavedFilters       File kept saved filters

##  General Notes:

   This is a major upgrade of FlowViewer. The upgrade preserves this 
   open-source option for netflow analysis in the age of IPFIX. The user
   is urged to read through the User's Guide for a better understanding
   of installation and configuration.

   Those who upgrade can preserve all previous filters and reports easily.
   Saved reports are automatically available in the new version. The only
   manual change requires users to run the 'convert_pre40_filters' script
   from the command line to move saved filters into the new format. Example:

   host>convert_pre40_filters .../FlowViewer_3.4/FlowViewer_SavedFilters

   With Respect to SiLK: The SiLK tool suite, developed by the NetSA group
   at Carnegie Mellon, is excellent software with equally excellent
   documentation. Version 3.0 of SiLK together with libfixbuf v1.1.0 are their
   entree into IPFIX/v9 netflow capture and analysis supporting IPv6. Initially
   they have chosen to limit the number of IPFIX Information Elements (IE) that
   the SiLK software will process. They have chosen a set that matches what
   flow-tools has provided with the addition of IPv6 data, but sadly with the
   exception of autonomous system (AS) elements. I have requested that they add
   the AS Elements, but we'll see. They have mentioned a future overhaul
   (beyond v3.0) to handle the entire IE space through user configuration. As
   of Spring 2013, SiLK v3.x is not fully through the process required to make
   the software open-source to the general public but they are proceeding with
   getting the approval. It is currently freely available to US Federal
   agencies.

   The FlowViewer_Configuration.pm file has changed:

    New parameters (configurable):

     $dashboard_directory     = "/var/www/html/FlowViewer_Dashboard";
     $dashboard_short         = "/FlowViewer_Dashboard";
     $silk_data_directory     = "/data/flows";
     $silk_bin_directory      = "/usr/local/bin";
     $sensor_config_directory = "/data/flows";
     @ipfix_devices           = ("Router_v9_1","Router_v9_2","Test_6509_v9");
     $sip_prefix_length       = "16";
     $dip_prefix_length       = "16";
     $silk_all_only           = "N";
     $left_title              = "Any Title You Like";
     $left_title_link         = "http://abc.com/";
     $right_title             = "Any Second Title You Like";
     $right_title_link        = "http://abc.com/";
     $recreate_cat_length     = 1*(60*60); # Time length of concatenated file
     $thumbnail_width         = 250; # probably should leave this alone
     $thumbnail_height        = 80;  # probably should leave this alone
     $filename_color          = "#CF7C29";
     $dig_forward             = "/usr/bin/dig +time=1 +tries=1 ";
     $default_identifier      = "DNS"; # "IP" for addresses; "DNS" for names

    Removed Parameters
    
     $bg_color                = "#FFFFFF";
     $text_color              = "#000000";
     $link_color              = "#000000";
     $vlink_color             = "#BF294D";
     $monitors_title          = "Your Company Name";
     $user_logo               = "Generic_Logo.jpg";
     $user_hyperlink          = "http://www.yourcompany.com/";

   With respect to the "Relay" scripts, many of you may already have resolved
   this issue by setting up a generic 'FlowViewer' directory and simply
   re-linking it to the new version's directory. I've been told this is proper
   :-). It certainly makes good sense. Otherwise the "Relay" approach is best
   explained below in Version 3.4 Release Notes.

# Version 3.4 Release Notes

  Update - 8/17/2011 - Fixed FlowViewer bug when requesting time periods just
  shy of midnight. This had already been fixed in FlowGrapher. Modifications
  were made to FlowViewer_Main.cgi.

  Update - 5/20/2011 - Modifications have been made to FlowGrapher_Main.cgi to
  fix a problem caused by the new speed-up processing. The speed-up was not
  accounting for Daylight Savings considerations.

  It's been awhile, so version 3.4 will fix a myriad of little problems which
  I mostly can't remember. The primary new capabilities include:

   1. In most cases, the user may now switch the device without losing entered
      filter criteria
   2. The different tool logos now provide a link to the Saved Reports page
   3. Users can now provide a meaningful name for saved FlowViewer and 
      FlowGrapher reports
   4. Fixes to an end-of-year problem have resulted in a 8% speed up of 
      FlowGrapher in general
   5. Users can select to limit FlowGrapher stats to no-zero data points, 
      if desired
   6. Fixed problems with sorting
   7. Corrected the graphing by 'flows' (was graphing 'flags' :-)
   8. Can now provide up to 20 source or destination IP address/address ranges
   9. Can now exclude specified IP addresses from a larger included address
      range

#  New Scripts and Files:

   FlowGrapherM.png            New logo link points to Saved reports web page
   FlowGrapherS.png            Revised logo link for naming of Saved Reports
   FlowViewerM.png             New logo link points to Saved reports web page
   FlowViewerS.png             Revised logo link for naming of Saved Reports
   FlowMonitorM.png            New logo link points to Saved reports web page
   flowcapture_restart         Renamed script for restarting flow-captures
   flowmonitor_restart         New script for re-starting FlowMonitor_Collector

#  General Notes:

   Remember to copy into the new directory (e.g., 
   /usr/lib/cgi-bin/FlowViewer_3.4) user logos, names file, as_names, 
   NamedInterfaces_Devices, NamedInterface_Exporters, FlowViewer_SavedFilters,
   etc., from the old cgi-bin directory.

   The simplest way to transition to the new version is to leave all 
   FlowViewer_Configuration.pm settings alone except:

    $reports_directory       = "/var/www/FlowViewer_3.4"; 
    $reports_short           = "/FlowViewer_3.4"; 
    $graphs_directory        = "/var/www/FlowGrapher_3.4"; 
    $graphs_short            = "/FlowGrapher_3.4"; 
    $monitor_directory       = "/var/www/FlowMonitor_3.4"; 
    $monitor_short           = "/FlowMonitor_3.4"; 
    $cgi_bin_directory       = "/usr/lib/cgi-bin/FlowViewer_3.4"; 
    $cgi_bin_short           = "/cgi-bin/FlowViewer_3.4"; 
    $work_directory          = "/usr/lib/cgi-bin/FlowViewer_3.4/Flow_Working"; 
    $names_directory         = "/usr/lib/cgi-bin/FlowViewer_3.4"; 
    $log_directory           = "/usr/lib/cgi-bin/FlowViewer_3.4"

   The following can remain the same (or copy contents to the new directory):

    $save_directory          = "/var/www/FlowViewer_Saves"; 
    $save_short              = "/FlowViewer_Saves"; 
    $filter_directory        = ".../FlowMonitor_Files/FlowMonitor_Filters";   
    $rrdtool_directory       = ".../FlowMonitor_Files/FlowMonitor_RRDtool";

   If this is an upgrade for you (e.g., from v3.3.1) I recommend using the
   FlowViewer_Relay.cgi, FlowGrapher_Relay.cgi, and the FlowMonitor_Relay.cgi
   scripts to alert users to the new version with links and a reminder to
   change their bookmarks. In each of the relay scripts tailor the following 
   line to your environment (point to the new FlowViewer_Configuration.pm file):

    require "/usr/lib/cgi-bin/FlowViewer_3.4/FlowViewer_Configuration.pm";

   ... then, in your old cgi-bin directory (e.g., FlowViewer_3.3.1), copy the
   following:

    cp FlowViewer_Relay.cgi FlowViewer.cgi
    cp FlowGrapher_Relay.cgi FlowGrapher.cgi
    cp FlowMonitor_Relay.cgi FlowMonitor.cgi
   
   Now, when users go to their book-marked FlowViewer web page, they will be
   directed to the new one. FlowMonitor_Relay.cgi is particularly important
   this is an upgrade it copies over archived FlowMonitors which would be a 
   bit tedious to copy by hand.

   The rsync_flows and rsync_monitors scripts are useful for easily backing
   up all raw netflow data and FlowMonitor state information (Filters and
   RRDtool databases.) The FlowViewer_CleanFiles script is useful for deleting
   aging files that are not necessary anymore. I run it out of 'cron' once a
   day.

   The performance_check script can be used from the command line to keep track 
   of how well your implementation is performing. I run it against my
   FlowMonitor_Collector.log file to see how things are going. Here at the
   NASA Earth Observing System network I have over 200 FlowMonitors and
   they complete in an average of 44 seconds. FlowMonitor_Collector runs every
   five minutes and I watch for runs that take longer than five minutes.
   Even in those situations, however, FlowMonitor_Collector seems to continue
   on with no real visible effects.

# Version 3.3 Release Notes

 ### Version 3.3.1 fixes a FlowMonitor_Collector bug when using exporters
     Also fixes problem for users without devices at all. If you are not
     using any devices (or exporters) you will now have to set:
     $no_devices_or_exporters = "Y"; Fixes FlowGrapher sorting of host
     names. This version fixes the problem of links to Trackings embedded
     in Group graphs not lining up properly. Fixes problem with 
     FlowMonitor_Grapher not printing out named interfaces. Fixes 
     FlowGrapher graph and output to now have exporter name. Fixes 
     problem with end-of-month graphs (missing days_in_month.)


# Change Log
 
 Version 3.4 - March 17, 2011
 
 See Version 3.4 Release Notes above

 Version 3.3
 
 See Version 3.3 Release Notes above

 Version 3.2
 
 Version 3.2 introduces Group monitors which are simply a monitor graph
 made up from the merging of several predefined Individual monitors onto
 one graph. There are no permanent RRDtool databases associated with a Group.
 In the construction of a group however, temporary RRDtool databases are 
 created to simulate how the Group will eventually look. The new script 
 FlowMonitor_Group uses RRDs.pm (comes with RRDtool) to generate these 
 transient databases quickly. Please ensure that your RRDs.pm module is 
 compatible with your RRDtool distribution (this should normally be the 
 case - but if you see "ERR: can't handle RRD file version 0003" in 
 DEBUG_GRAPHER, you'll need to upgrade your RRDs.pm.

 This version also includes a 'speed-up' for FlowMonitor_Collector which
 now concatenates once for each device. This is controlled by 
 $use_existing_concats, which defaults to "Y".

 Version 3.2 will continue to work happily along with earlier version
 exiting Filter and RRDtool files. There are no new FlowViewer_Configuration
 parameters of consequence. There are now two types of monitors; Individual
 and Group. The FlowMonitor input screen will default to Individual which is
 the same as the existing monitors.

 Woj Kozicki has contributed an Autonomous System (AS) resolving capability 
 and it is included in v 3.2.

 New FlowViewer, FlowGrapher, and FlowMonitor logos have been developed for 
 version 3.2. These new logos provide embedded links to the other tools so that
 the user can switch between them easily and retain input parameters.

 Version 3.1
 
 1. Added MIN, MAX, AVG, 95th PCT to FlowGrapher
 2. Added ability to 'archive' monitors
 3. Added ability to enter port ranges separated by a colon (:)
 4. Can now use any mask length for networks (1 - 32)
 5. Added RRDgraph 'lazy-mode' option to speed up graphing
 6. Upgraded FlowViewer/Grapher ability to go back more than 30 days
 7. Added the ability to configure file permissions
 8. Improved speed of FlowGrapher for larger values of 'detail lines'
 9. Fixed $rrdtool_bin-directory variable name
 10. Added ability to retain intermediate files for debugging
 11. Sorted list of Active Trackings
 12. Fixed bug where non-zero 'cutoof lines' would supress some reports
 13. Fixed FlowViewer rate output to calculate average from all flows
 14. Fixed FlowMonitor_Collector log output (to collect_period_average)

 Version 3.0
 
 1. Major new addition of FlowMonitor
 2. Reorganized scripts so that the date and time fields are updated
    with each invocation
 3. Moved common code (e.g., filter creation) to FlowViewer_Utilities.pm
 4. Improved Report Parameters output formatting
 5. Provided host names capability for FlowGrapher (thanks Mark Foster)
 6. Introduced debug and logging capabilities
 7. Merged GDBM/NDBM into a single script (thanks Ed Ravin)
 
 Version 2.3

 1. Modified FlowGrapher record processing to not call 'timelocal' for
    epoch times. Other speed improvements. Result: up to 10 times faster.
 2. FlowGrapher error leaving spikes is fixed (thanks Mark Foster)
 3. Bug with concatenation when $N=0 fixed (thanks Dave Faught)

 Version 2.2

 1. Added flow_select parameter to control which flows are considered
    with respect to the specified time period
 2. Removed Easterm Time (ET) notation. All times are system local

 Version 2.1

 1. Fixed concatenation. Needs to start one flow file length before start time
 2. Fixed end-of-year problem in FlowGrapher
 3. Small problem for time requests that end just before midnight

 Version 2.0

 1. Used pipe (|) instead of reading intermediate files (thanks Woj Kozicki!)
 2. Introduced configurable variable $N specifies flow-directory nesting levels
 3. Reduced default value of variable $flow_capture_interval to 1800
 4. Created FlowViewer_NDBM.cgi for users whose Perl does not have GDBM
 5. Created configurable 'work_directory' separate from cgi_bin_directory
 6. Sped up concatenation for requests that cross day boundaries
 7. Added filter fields: Protocol, TOS Field, TCP Flags
 8. Added some more syntax checking
 9. Added FlowGrapher capability (requires GD for Perl)

 Version 1.0 (Original)


# New Capabilities
#
#   1. Some devices will now have 'named interfaces' (thanks C. Kishimoto)
#   2. The user can now save filters of interest and recall them later
#   3. Data can now be analyzed by Exporter ID (in addition to device name)
#   4. Users can now set thresholds on FlowMonitors, and be alerted
#   5. Users can now sort FlowGrapher output based on column type
#   6. FlowViewer now provides Pie Charts
#   7. Capability added to apply a Sampling Multiplier to output
#   8. FlowMonitors now have a '3 year' graph
#   9. The user can now generate text listings of FlowMonitor output
#  10. Filtering on next-hop has been added
#  11. Logging has been made more flexible (e.g., less data)
#  12. Preserve latest three notations (was keeping first three)
#  13. Can now specify and display time-zones
#  14. A hook has been provided for a User Logo with link out of FlowViewer
#  15. New file cleanup scripts have been added
#  16. Unit Conversion capability has been added (thanks C. Kishimoto)
#  17. Can now graph Flows, Packets as well as Octets (thanks E.Lautenschlaeger)
#  18. Improved AS name resolution (thanks S. Cardus)
#  19. New saved_directory for storing saved Reports and Graphs.
#
# New Scripts and Files:
#
#  FlowGrapher_Sort.cgi        Sorts FlowGrapher Detail Lines by column
#  FlowMonitor_Dumper.cgi      Invoked by link in Trackings, prints text values
#  FlowViewer_Save.png         New logo with links for saving filters, reports
#  Flowgrapher_Save.png        New logo with links for saving filters, reports
#  FlowViewer_CleanASCache     Tool used to remove obsolete AS name resolutions
#  FlowViewer_CleanFiles       Tool used to remove old intermediate files
#  FlowViewer_CleanHostCache   Tool used to remove obsolete host name resolutions
#  FlowViewer_Relay.cgi        Optional: points users to new version (see Notes)
#  FlowGrapher_Relay.cgi       Optional: points users to new version (see Notes)
#  FlowMonitor_Relay.cgi       Optional: points users to new version (see Notes)
#  flowcap                     Optional start-up script for flow-tools and 
#                              FlowMonitor
#
#  NamedInterfaces_Devices     Holds interface names for SNMP device indices
#  NamedInterfaces_Exporters   Holds interface names for SNMP index (exporters)
#  FlowViewer_SavedFilters     Created during processing to hold saved filters
