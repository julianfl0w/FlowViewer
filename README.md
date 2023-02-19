# README (this file) FlowViewer V4.6  Date: 01/26/2015

FlowViewer is a set of three tools (FlowViewer, FlowGrapher,
FlowMonitor) that create text reports, graph reports, and 
long-term monitor reports from flow-tools and SiLK captured
and stored netflow data. FlowViewer can run with both flow-tools
and SiLK simultaneously. Flow-tools can handle up to v7; SilK 
can handle v5, v9, and IPFIX. The User's Guide is very helpful.

## Software Dependencies:


- flow-tools  http://code.google.com/p/flow-tools (If collecting v5 only)
- SiLK        http://tools.netsa.cert.org/silk    (If collecting IPFIX)
- libfixbuf   http://tools.netsa.cert.org/silk    (If collecting IPFIX)
- gd          http://www.libgd.org/Downloads
- GD          http://search.cpan.org/~lds/GD-2.30
- GD::Graph   http://search.cpan.org/~mverb/GDGraph-1.43
- GD::Text    http://search.cpan.org/~mverb/GDTextUtil-0.86/Text
- RRDtool     http://people.ee.ethz.ch/~oetiker/webtools/rrdtool/pub

## Quick Upgrade

0. If using SiLK, must upgrade to v3.8.0 or newer
1. Untar the package into a new cgi-bin subdirectory
2. Configure FlowViewer_Configuration.pm variables to your environment
   and create all necessary directories with proper permissions
3. Replace old logos with new buttons (will be done automatically)
4. Copy FlowViewer.css, FlowViewer.pdf to $reports_directory
5. Configure FlowViewer_Configuration.pm to point to existing 
   FlowMonitor_Filter and FlowMonitor_RRDtool directories
   [For prior v4.5 you can rename these directories to:
   FlowMonitor_Filter and FlowMonitor_RRDtool or change the 
   configuration variables to point to the old directories.]
6. Configure new FlowViewer_Configuration.pm
7. Stop old FlowMonitor_Collector and FlowMonitor_Grapher
   [Upgrading from v4.4 and prior you will be stopping FlowTracker_Collector
   and FlowTracker_Grapher.]
8. Start new FlowMonitor_Collector and FlowMonitor_Grapher
9. Copy NamedInterfaces_Devices, names file, user logo to new directory
0. (If upgrading from pre v4.0) Run convert_pre40_filters against existing filters 
   (ie FlowViewer_SavedFilters)
1. Use included 'User Relay' scripts if desired (recommended - see below)

## Quick Install


   1. Untar into cgi-bin subdirectory

   For netflow v5 and older (option):

   2. Download, install, configure flow-tools

   For IPFIX (e.g., v9 - also handles v5):

   3. Download, install, configure SiLK (v3.8.0 or newer) and libfixbuf

   For sflow

   4. From SiLK FAQ:

      "Support for sFlow v5 is available as of SiLK 3.9.0 when you configure
      and build SiLK to use v1.6.0 or later of the libfixbuf library."

###   For FlowViewer

   5. Configure FlowViewer_Configuration.pm variables as necessary
   6. Create all necessary directories with proper permissions
   7. Copy FlowViewer.css, FlowViewer.pdf to $reports_directory
   8. Point browser to FV.cgi

###   For FlowGrapher

   9. Install gd (C), GD (Perl), GD::Graph (Perl) GD::Text (Perl)
  10. Configure FlowViewer_Configuration.pm variables as necessary
  11. Point browser to FV.cgi

###   For FlowMonitor

  12. Install RRDtool (at least version 1.4)
  13. Create FlowMonitor_Filter and FlowMonitor_RRDtool directories
  14. Configure FlowViewer_Configuration.pm variables as necessary
  15. Start FlowMonitor_Collector, FlowMonitor_Grapher in background
  16. Point browser to FV.cgi

###   For all FlowViewer tools
  
  17. Review all FlowViewer directories and files for proper permissions

## Notes:

  Many thanks to Carles Kishimoto, Eric Lautenschlaeger, and Sean Cardus for 
  their ideas and code contributions. Thanks to Dario La Guardia for pointing
  out a graphing problem that turned out to be a rounding error in FlowGrapher.
  Credit to Peter Hoffswell for the idea of linking the tools.

  There are no new software dependencies with FlowViewer version 3.3.1, however
  Named Interfaces now requires Javascript in the browser to operate.

  If you are having trouble with creating Tracking Groups, you may have a
  problem with the installation of RRDs.pm. This needs to be placed in a
  library that Perl includes in it's @INC array. For a fix, see the FlowViewer
  FAQ on the web site.

  Using the 'Relay' scripts (these are optional)

   If you have other users and you would like to point them to the new version,
   copy the included 'Relay' scripts over the old FlowViewer.cgi,
   FlowGrapher.cgi and FlowMonitor.cgi scripts in the last version's directory.

   For example:

    In the old directory /htp/cgi-bin/FlowViewer_3.2:

    mv FlowViewer_Relay.cgi  FlowViewer.cgi
    mv FlowGrapher_Relay.cgi FlowGrapher.cgi
    mv FlowMonitor_Relay.cgi FlowMonitor.cgi
   
   Then, when the user goes to the old FlowViewer, he will be provided a link
   to the new FlowViewer, and asked to change his bookmarks.


  Setting up crontab file for cleaning FlowViewer files:
      
   min   hr  dom  moy  dow    command

     5   0    *    *    *     .../FlowViewer_3.4/FlowViewer_CleanFiles
                                > .../FlowViewer_3.4/cleanup.log 
                                2 >> .../FlowViewer_3.4/cleanup.log

  The file cleanup is controlled by parameters in FlowViewer_Configuration.pm:

   $remove_workfiles_time   = 86400;     
   $remove_graphfiles_time  = 7*86400; 
   $remove_reportfiles_time = 7*86400;

  Remember, whichever crontab account this is started from must have adequate 
  permissions to remove files created by the web process owner (e.g., apache.)

## Documentation

 The FlowViewer User's Guide is available on the FlowViewer Website:

   http://ensight.eos.nasa.gov/FlowViewer

## Dependencies

 - FlowGrapher requires the Perl GD and GD:Graph packages
   gd package. Thomas Boutrell's graphics package written in 'C'
   GD package: http://search.cpan.org/~lds/GD-2.30/
   GD::Graph package: http://search.cpan.org/~mverb/GDGraph-1.43/
 - FlowViewer.cgi requires the GDBM or NDBM capability in Perl
 - FlowMonitor requires RRDtool (at least version 1.2.12)
   RRDtool: http://oss.oetiker.ch/rrdtool

## Contents

### FlowViewer_Configuration.pm

 This file contains parameters that configure and control the 
 FlowViewer, FlowGrapher, and FlowMonitor environments. This package 
 should remain in the same directory that the CGI scripts are in.

### FlowViewer_Utilities.pm

 This file contains processing used by multiple programs (e.g., to
 create the Report Parameters output for each tool, and other utilities
 (e.g., 'epoch_to_date' which converts between typical date formats 
 and 'seconds since 1972') that are invoked by other scripts. This 
 package should be placed in the same directory as the CGI scripts.

### FlowViewer.cgi

 This script produces the web page which provides the user the form
 for entering analysis selection criteria for FlowViewer. Version 3.0
 reorganized the processing. FlowViewer.cgi is now the old
 create_FlowViewer_webpage. This change permits the input date and time
 to be updated with each invocation.

### FlowViewer_Main.cgi

 This script responds when the user completes the selection criteria
 form and submits the 'Generate Report' command. The script creates a
 flow-tools filter file based on the selection criteria. Based on the
 input time period, the script concatenates the relevant flow-tools
 data files for the selected device. The location of the flow-tools
 raw data files is specified via the 'flow_data_directory' parameter.
 The script then invokes the selected statistics/print report flow-tools
 program and reformats the output into HTML. An option is available in
 FlowViewer_Configuration to have this script use the NDBM capability
 (for caching resolved host names) instead of the default GDBM
 capability for users whose Perl distribution does not have GDBM.

### FlowGrapher.cgi

 This script produces the web page which provides the user the form
 for entering analysis selection criteria for FlowGrapher. Version 3.0
 reorganized the processing. FlowGrapher.cgi is now the old
 create_FlowGrapher. This change permits the input date and time
 to be updated with each invocation.

### FlowGrapher_Main.cgi

 This script responds when the user completes the FlowGrapher selection
 criteria form and submits the 'Generate Graph' command. The script
 creates intermediate processing files exactly like FlowViewer above.
 The script then parses intermediate output, fills time buckets, and
 generates a graphic image. Textual output accompanies the graph. An
 option is available in FlowViewer_Configuration to have this script use
 the NDBM capability (for caching resolved host names) instead of the
 default GDBM capability for users whose Perl distribution does not have
 GDBM.

### FlowGrapher_Sort.cgi

 This script is invoked when the user clicks on a column header for the
 Detail Lines of a FlowGrapher report. The textual data on the page is
 sorted and re-presented.

### FlowGrapher_Colors

 This file contains a translation between textual color names and their
 RGB value counterparts.

### FlowMonitor.cgi

 This script produces the web page which provides the user the form
 for entering analysis selection criteria for FlowMonitor. The script
 also provides the user with the ability to review, revise, or remove
 existing monitors. FlowMonitor was new in version 3.0. 

### FlowMonitor_Main.cgi

 This script responds when the user completes the FlowMonitor selection
 criteria form and submits the 'Establish Tracking' command. The script
 responds to the users desire to create, remove, or revise a monitor.

### FlowMonitor_Group.cgi

 This script controls the building of groups from existing Individual
 FlowMonitors. The user has the ability with FlowMonitor v3.2 to create
 'groups' from pre-defined Individual monitors. A Group Tracking has no
 RRD database associated with it, but simply creates a multifaceted graph
 from several existing monitors. The Group 'merges' the Individual graphs
 onto a single graph.

### FlowMonitor_Dumper.cgi

 This script is invoked when the user clicks on a link within the
 FlowMonitor graph labeled '[List values]'. The script dumps the 
 RRDtool contents onto a web page.

### FlowMonitor_Collector

 The script is started once by the user and placed in the 'background'.
 The script will execute and then sleep for the duration of a five minute
 period, essentially running every five minutes. For each existing monitor, 
 the script applies the associated filter to the flow data and extracts the
 amount that occured during a 5-minute window approximately 30 miuntes
 earlier. This is to permit long-running flows to have been exported and 
 available to the collector. The script then divides the total bits by 
 300 seconds to get an average bits-per-second rate during the period.
 The data point is then provided to RRDtool for storage. The script
 should be started out of the cgi-bin directory.

### FlowMonitor_Grapher
 
 The script is started once by the user and placed in the 'background'.
 The script will execute and then sleep for the duration of a five minute
 period, essentially running every five minutes. The script runs the
 RRDtool graph function for each existing monitor. Daily, weekly,
 monthly, and yearly graphs are updated with the latest information. The
 script creates an html page for each monitor that includes the filter
 parameters and the four graphs. The script also creates an overall web
 page ($monitor_webpage) that provides links to all active monitor pages.
 The script should be started out of the cgi-bin directory.

### FlowViewerM.png

 The FlowViewerM logo with links. Leave this file in the 'cgi-bin_directory', 
 the FlowViewerM.cgi script will place a copy of the image in 
 'html_directory'. This image contains mapped links to FlowGrapher and
 FlowMonitor such that those input pages are pre-loaded with the filter
 criteria from FlowViewer.

### FlowViewerS.png

 The FlowViewerS logo with links. Leave this file in the 
 'cgi-bin_directory', the FlowViewerS.cgi script will place a copy of the 
 image in 'reports_directory'. This image contains mapped links to the other
 tools as well as links for saving the filter used or the report generated.

### FlowGrapherM.png

 The FlowGrapherM logo with links. Leave this file in the 'cgi-bin_directory', 
 the FlowGrapherM.cgi script will place a copy of the image in 
 'graphs_directory'. This image contains mapped links to FlowViewer and
 FlowMonitor such that those input pages are pre-loaded with the filter
 criteria from FlowGrapher.

## FlowGrapherS.png

 The FlowGrapherS logo with links. Leave this file in the 
 'cgi-bin_directory', the FlowGrapherS.cgi script will place a copy of the 
 image in 'graphs_directory'. This image contains mapped links to the other
 tools as well as links for saving the filter used or the report generated.

### FlowMonitorM.png

 The FlowMonitor logo with links. Leave this file in the 'cgi-bin_directory', 
 the FlowMonitorM.cgi script will place a copy of the image in 
 'monitor_directory'. This image contains mapped links to FlowViewer and
 FlowGrapher such that those input pages are pre-loaded with the filter
 criteria from FlowMonitor.

### FlowViewer_Save.cgi

 This script moves temporary save files into a permanent residence
 as defined by either the 'reports_directory' or 'graphs_directory'
 environment variables.

### FlowViewer_CleanFiles
 
  A utility for cleaning out temporary files that have been left
  over from debugging (e.g. $debug_files = 'Y'). Files older than
  the following configurable parameters are removed:
  
   $remove_workfiles_time   = 86400; 
   $remove_graphfiles_time  = 7*86400; 
   $remove_reportfiles_time = 7*86400

  See above for crontab settings for running this automatically.

### FlowViewer_CleanASCache
 
 A utility for cleaning out from the AS resolving cache ($as_file) a 
 resolved AS name that is no longer valid.
 
### FlowViewer_CleanHostCache
 
 A utility for cleaning out from the DNS resolving cache ($names_file) 
 a resolved host name that is no longer valid.
 
### FlowViewer_Relay.cgi, FlowGrapher_Relay.cgi, FlowMonitor_Relay.cgi

 Short scripts that refer users from version 3.3.1 to version 3.4. This
 keeps you from having to notify users to go to a different web site.

### flowcapture_restart

 A shell script used for starting up and restarting flow-captures. Tailor 
 this for your environment.

### flowmonitor_restart

 A shell script used for starting up and restarting FlowMonitor_Collector
 and FlowMonitor_Grapher. Tailor this for your environment.

### Generic_Logo.jpg

 This image is to be replaced by your own image that can point back to
 anywhere (e.g., your overarching NMS system.)

### NamedInterfaces_Devices
 
 This file is used for SNMP index to named interface translation. This file 
 provides translation when you are saving data by individual devices. Examples
 are provided.

### NamedInterfaces_Exporters
 
 This file is used for SNMP index to named interface translation. This file 
 provides translation when you are saving data into a single directory but
 for (possibly) multiple devices differentiated by EXPORTER_ID. Examples are
 provided.

### Configuration parameters

 The FlowViewer, FlowGrapher, and FlowMonitor scripts all use parameters
 in the FlowViewer_Configuration.pm file to control the environment that
 they run in. Here is a brief explanation of some of the relevant 
 parameters:

 $ENV(PATH) - modify as appropriate for your installation
 $FlowViewer_server - IP address of server hosting this software
 $FlowViewer_service - Either HTTP (port 80) or HTTPS (port 443)
 $reports_directory - Directory to hold saved FlowViewer reports
 $reports_short - Reports directory beginning from web server default
 $graphs_directory - Directory to hold saved FlowGrapher reports
 $graphs_short - Graphs directory beginning from web server default
 $monitor_directory - Directory to hold FlowMonitor monitors
 $monitor_short - Monitor directory beginning from web server default
 $filter_directory - Directory in which to keep FlowMonitor filter files
 $rrdtool_directory - Directory in which to keep FlowMonitor RRDtool files
 $cgi_bin_directory - Directory which holds cgi scripts
 $cgi_bin_short - cgi-bin directory beginning from web server default
 $flow_data_directory - Directory that holds all flow-tools data files
 $exporter_directory - Directory where netflow stored for multiple exporters
 $flow_bin_directory - Directory where all flow-tools reside
 $rrdtool_bin_directory - Location of RRDtool programs
 $work_directory - Directory to store intermediate files 
 $names_directory - Directory to save permanent 'names' file
 $flow_capture_interval - Interval beyond end point to capture all flows
 $flow_file_length - Length (in seconds) of each of your flow files 
 $devices - List of device names exporting netflow (see #4 below)
 $no_devices_or_exporters - Set to "Y" if you have no devices and no exporters
 $N - Used to control directory organization (see #5 below)
 $dig - Location of DNS utility 'dig' (set to nslookup if required)
 $actives_webpage - Name of HTML file which will list your Trackings
 $monitors_title - Title for HTML page which lists Trackings
 $user_logo - Filename of image used for your logo
 $user_hyperlink - Link associated with $user_logo
 $use_even_hours - Will start default time periods at the top of the hour
 $use_NDBM - Some Perls don't have GDBM (default), but do have NDBM
 $start_offset - Offset from current time for beginning pre-loaded time period
 $end_offset - Offset from current time for end of pre-loaded time period
 $flow_capture_interval - Minutes beyond end period for collecting all flows
 $flow_file_length - Size (minutes) of each flow-tools flow file (default = 15)
 $labels_in_titles - Whether to print FlowMonitor title in the graph itself
 $debug_files - If Yes, will not remove intermediate files
 $collection_offset - Seconds into past to begin collection period
 $collection_period - Period to examine for FlowMonitor (keep at 5 minutes!)
 $use_existing_concats - DEPRECATED. Re-use concatenations (much faster)
 $rrd_dir_perms - (And others) UNIX directory or file permissions

 The rest of this file contains basic parameters such as colors, etc. Each
 parameter is dicussed in more detail in the User's Guide.

### Additional Considerations

 1. Directory permissions for the subdirectories created for the
 'htdocs', 'work', 'names', 'cgi-bin' (e.g., FlowMonitor_Filter, 
 FlowMonitor_RRDtool) directories must permit the owner of the web
 server process (e.g., apache) to write into these directories.
 The directories may have been created by a different user. Version 3.0
 introduced the use of $dir_perms. There are several of these included
 in FlowViewer_Configuration.pm. These are the permissions that the
 scripts will set your various FlowViewer files and directories to.
 They default to '0777' which permits the open interaction between the
 web server process owner and the FlowMonitor background process owner.
 You may want to adjust these permissions differently according
 to your security policies, and whether you use the same or different
 accounts for the web and background processes.

 2. FlowViewer and FlowGrapher offer the ability to save interesting
 reports. To do this, the scripts save a temporary copy of every report
 in advance of the user electing to save it permanently. These 
 intermediate files will accumulate in the 'work' directory specified
 in the FlowViewer_Configuration file. These files could be removed 
 daily via a cron script to prevent unecessary use of disk space. When
 the user elects to save a report, it is copied into either the 
 'reports_directory', or the 'graphs_directory' depending on which 
 function he is running. See discussion of FlowViewer_CleanFiles above.

 3. FlowViewer and FlowGrapher offer the ability to resolve NetFlow IP
 addresses into their host names on the fly. This process is speeded
 up by caching names into a 'names' file which resides in the directory
 specified by the 'names_directory' parameter. This parameter defaults
 to /tmp, but this may not be the best directory for you since it will
 disappear with a reboot. As you are building up your 'names' file
 with early runs, you will notice the speed increase dramatically
 as the 'names' file is used more. The process of resolving names is
 the primary reason for slower overall FlowViewer performance. You
 should preferably use the GDBM array database which is fastest.
 However, not all Perl distributions support GDBM but most do support
 NDBM. The '$use_NDBM' flag in FlowViewer_Configuration.pm will 
 cause the FlowViewer_Main and FlowGrapher_Main scripts to use NDBM.

 4. The FlowViewer and FlowGrapher reporting features use a flow-tools
 data directory layout that has a particular device at the top. A
 typical flow-tools directory looks like:

 /flows/router_1/2005/2005-07/2005-07-04

 The device name (router_1) is obtained from an array called 'devices'
 in the FlowViewer_Configuration.pm file. Populate this array with your
 device names. If your flow-data file structure does not include a
 device name, for example you are collecting only from one device, set
 the @devices array to empty (i.e., @devices = ("");) On the web page
 you can ignore the Devices pulldown selection.

 As of version 3.3, users may now apply FlowViewer to directories that 
 collect from multiple sources, differentiated by EXPORTER_ID. If you
 are using this method (i.e., all flow-captures going into a single
 directory), simply set the $exporter_directory parameter to the
 directory that is set up to store the flow-data files.

 5. Different organizations store captured netflow data differently
 according to the 'N" setting on the flow-capture statement. However,
 there is a bug in the flow-tools documentation such that the default
 value is truly '3' and not '0' as indicated. I have set $N = 3 to
 reflect the more common setting. The directory structure associated 
 with $N = 3 is shown below:

 /flows/router_1/2005/2005-07/2005-07-04

 If you are not seeing output, please check this setting.

 6. Version 3.2 introduces Groups. Intermediate RRDtool databases are 
 created on the fly in order to create a temporary FlowMonitor graph that
 shows the user how the final graph will look. It uses the Perl RRDs.pm
 RRDtool module to speed this up. Make sure your RRDtool distribution
 has a compatible RRDs.pm module.

# Vital Assistance

 Special thanks to those FlowViewer users who provided feedback and valuable
 suggestions, including Sejin Ahn, Mark Boolootian, Bogdan Ghita, Woj Kozicki,
 Ed Arvin, Alex Shepherd, Mike Smith, Scott Wingfield, Vali Magdalinoiu, Ed
 ravin, Eric Lautenschlaeger, Sean Cardus, Carles Kishimoto, Shigeki Taniguchi,
 Dave Faught, Peter Hofwell, Dario La Guardia, Mike Stowe, Chris Spitzlay and
 Mike Donnelly. Big thanks to fellow toiler in the NASA vineyard Mark Foster
 for some detailed testing, excellent suggestions, and code to go along with it.
 Thanks from all of us to NASA whose unending support of innovation in all fields
 has resulted in this toolset.

# Bugs, recommendations

 If you need help installing, have a question, discover a bug, or have a 
 recommendation, please send an email to:

 Joe Loiacono
 jloiacon@csc.com

 FlowViewer is being developed at NASA by a contractor in the employ of the
 United States Federal Government in the course of his official duties.
 Pursuant to Title 17, Section 105 of the United States Code, this software is
 not subject to copyright protection and is in the public domain. FlowViewer is
 an experimental system. NASA assumes no responsibility whatsoever for its use
 by other parties, and makes no guarantees, expressed or implied, about its
 quality, reliability, or any other characteristic.
