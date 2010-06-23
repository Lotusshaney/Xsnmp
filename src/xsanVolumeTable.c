/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 15999 2007-03-25 22:32:02Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <fcntl.h>
#include <pcre.h>
#include "util.h"
#include "command.h"
#include "xsanVolumeTable.h"

static struct timeval vollist_cache_timestamp = { 0, 0 };
static int last_index_used = 0;

#define MAX_CACHE_AGE 10
#define OVECCOUNT 90

/** Initializes the xsanVolumeTable module */
void
init_xsanVolumeTable(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_xsanVolumeTable();
}

/** Initialize the xsanVolumeTable table by defining its contents and how it's structured */
void
initialize_table_xsanVolumeTable(void)
{
    static oid xsanVolumeTable_oid[] = {1,3,6,1,4,1,20038,2,1,1,1};
    size_t xsanVolumeTable_oid_len   = OID_LENGTH(xsanVolumeTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    reg = netsnmp_create_handler_registration(
              "xsanVolumeTable",     xsanVolumeTable_handler,
              xsanVolumeTable_oid, xsanVolumeTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: xsanVolumeIndex */
                           0);
    table_info->min_column = COLUMN_XSANVOLUMEINDEX;
    table_info->max_column = COLUMN_XSANVOLUMEPORT;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = xsanVolumeTable_get_first_data_point;
    iinfo->get_next_data_point  = xsanVolumeTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

    /* Typical data structure for a row entry */
struct xsanVolumeTable_entry {
    /* Index values */
    long xsanVolumeIndex;

    /* From 'fsmlist' */
    char *xsanVolumeName;
    size_t xsanVolumeName_len;
    long xsanVolumeFSSIndex;
    long xsanVolumeState;
    char *xsanVolumeStateMessage;
    size_t xsanVolumeStateMessage_len;
    char *xsanVolumeLastAdminAction;
    size_t xsanVolumeLastAdminAction_len;
    char *xsanVolumeLastTermination;
    size_t xsanVolumeLastTermination_len;
    u_long xsanVolumeLaunchCount;
    u_long xsanVolumeCoreDumpCount;
    char *xsanVolumeFlags;
    size_t xsanVolumeFlags_len;

    /* From 'show long' */
    char *xsanVolumeCreated;
    size_t xsanVolumeCreated_len;
    u_long xsanVolumeActiveConnections;
    u_long xsanVolumeFsBlockSize;
    u_long xsanVolumeMsgBufferSize;
    u_long xsanVolumeDiskDevices;
    u_long xsanVolumeStripeGroups;
    u_long xsanVolumeTotalKBlocks;
    u_long xsanVolumeFreeKBlocks;
    U64 xsanVolumeTotalBlocks;
    U64 xsanVolumeFreeBlocks;
    u_long xsanVolumeUtilization;
    
    /* From 'fsmlist' */
    u_long xsanVolumePid;
    u_long xsanVolumePort;
    
    /* Obsolescence check */
    time_t last_seen;
    struct timeval volume_cache_timestamp;

    /* Illustrate using a simple linked list */
    int   valid;
    struct xsanVolumeTable_entry *next;
};

struct xsanVolumeTable_entry  *xsanVolumeTable_head;

/* create a new row in the (unsorted) table */
struct xsanVolumeTable_entry *
xsanVolumeTable_createEntry(long  xsanVolumeIndex) {
    struct xsanVolumeTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct xsanVolumeTable_entry);
    if (!entry)
        return NULL;

    entry->xsanVolumeIndex = xsanVolumeIndex;
    entry->next = xsanVolumeTable_head;
    xsanVolumeTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
xsanVolumeTable_removeEntry( struct xsanVolumeTable_entry *entry ) {
    struct xsanVolumeTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = xsanVolumeTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        xsanVolumeTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
xsanVolumeTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    *my_loop_context = xsanVolumeTable_head;
    return xsanVolumeTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
xsanVolumeTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct xsanVolumeTable_entry *entry = (struct xsanVolumeTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->xsanVolumeIndex );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}

void update_vollist ()
{
     struct timeval now;
     gettimeofday (&now, NULL);

     // char *data = x_command_run("cvadmin -e 'fsmlist'", 0);
     // if (!data) return;
     // x_printf ("data is %p", data);
     // size_t data_len = strlen(data);

     // Debug
     int fd;
     fd = open ("fsmlist_example2.txt", O_RDONLY);
     char *data = malloc (65536);
     size_t data_len =  read (fd, data, 65535);
     data[data_len] = '\0';     
     
     const char *error;
     int erroffset;
     int ovector[OVECCOUNT];
     pcre *re = pcre_compile("^\\*?([^ \\[\\n]+)\\[(\\d+)\\](?:[ ]+port (\\d+), pid (\\d+))?.*\\n[ ]*State: (\\w+) (.*)\\n(?:[ ]*Last Admin: ([^\\n]+)\\n)?(?:[ ]*Last Termination: ([^\\n]+)\\n)?(?:[ ]*Launches (\\d+), core dumps (\\d+), flags (.*))$", PCRE_MULTILINE, &error, &erroffset, NULL);

     if (re == NULL) { x_printf ("update_vollist failed to compile regex for wired"); free (data); return; }

     ovector[0] = 0;
     ovector[1] = 0;
     while(1)
     {
         int options = 0;                 /* Normally no options */
         int start_offset = ovector[1];   /* Start at end of previous match */

         if (ovector[0] == ovector[1])
         {
             if (ovector[0] == data_len) break;
         }

         int rc = pcre_exec(
                             re,                   /* the compiled pattern */
                             NULL,                 /* no extra data - we didn't study the pattern */
                             data,              /* the subject string */
                             data_len,       /* the length of the subject */
                             start_offset,         /* starting offset in the subject */
                             options,              /* options */
                             ovector,              /* output vector for substring information */
                             OVECCOUNT);           /* number of elements in the output vector */
                         
         x_printf ("rc=%i", rc);

         if (rc == PCRE_ERROR_NOMATCH)
         {
             x_printf ("No match");
             if (options == 0) break;
             ovector[1] = start_offset + 1;
             continue;    /* Go round the loop again */
         }

         /* Other matching errors are not recoverable. */
         if (rc > 0)
         {
             /* Matched an Xsan Volume. Vectors:
              * 0=FullString 1(2)=Volume 2(4)=Index 3(6)=Port 4(8)=Pid
              * 5(10)=State 6(12)=StateMessage 
              * 7(14)=LastAdminMessage 
              * 8(16)=LastTerminationMessage
              * 9(18)=Launches 10(20)=Core Dumps 11(22)=Flags
              */
             char *volname_str;
             asprintf (&volname_str, "%.*s", ovector[3] - ovector[2], data + ovector[2]);
             trim_end(volname_str);

             struct xsanVolumeTable_entry *entry = xsanVolumeTable_head;
             while (entry)
             {
                 if (!strcmp(volname_str, entry->xsanVolumeName)) break;
                 entry = entry->next;
             }
             if (!entry)
             {
                 last_index_used++;
                 entry = xsanVolumeTable_createEntry(last_index_used);
                 entry->xsanVolumeName = strdup(volname_str);
                 entry->xsanVolumeName_len = strlen (entry->xsanVolumeName);
             }
             entry->last_seen = now.tv_sec;
             free (volname_str);
             volname_str = NULL;
             extract_string_in_range (data + ovector[12], ovector[13] - ovector[12], &entry->xsanVolumeStateMessage, &entry->xsanVolumeStateMessage_len);
             entry->xsanVolumeFSSIndex = extract_uint_in_range (data + ovector[4], ovector[5] - ovector[4]);
             entry->xsanVolumePort = extract_uint_in_range (data + ovector[6], ovector[7] - ovector[6]);
             entry->xsanVolumePid = extract_uint_in_range (data + ovector[8], ovector[9] - ovector[8]);
             char *state_str;
             asprintf (&state_str, "%.*s", ovector[11] - ovector[10], data + ovector[10]);
             if (!strcmp(state_str, "REGISTERED")) entry->xsanVolumeState = 1;
             else if (!strcmp(state_str, "STOPPED")) entry->xsanVolumeState = 2;
             else entry->xsanVolumeState = 0;
             free (state_str);
             extract_string_in_range (data + ovector[12], ovector[13] - ovector[12], &entry->xsanVolumeStateMessage, &entry->xsanVolumeStateMessage_len);
             extract_string_in_range (data + ovector[14], ovector[15] - ovector[14], &entry->xsanVolumeLastAdminAction, &entry->xsanVolumeLastAdminAction_len);
             extract_string_in_range (data + ovector[16], ovector[17] - ovector[16], &entry->xsanVolumeLastTermination, &entry->xsanVolumeLastTermination_len);
             entry->xsanVolumeLaunchCount = extract_uint_in_range (data + ovector[18], ovector[19] - ovector[18]);
             entry->xsanVolumeCoreDumpCount = extract_uint_in_range (data + ovector[20], ovector[21] - ovector[20]);
             extract_string_in_range (data + ovector[22], ovector[23] - ovector[22], &entry->xsanVolumeFlags, &entry->xsanVolumeFlags_len);
         }
         else
         {
             x_printf("Matching error %d\n", rc);
             pcre_free(re);    /* Release memory used for the compiled pattern */
             return;
         }  
     }
     
     pcre_free(re);    /* Release memory used for the compiled pattern */

     /* Check for obsolete entries */
     struct xsanVolumeTable_entry *entry = xsanVolumeTable_head;
     while (entry)
     {
         struct xsanVolumeTable_entry *next = entry->next;
         if (entry->last_seen != now.tv_sec)
         {
             /* Entry is obsolete */
             xsanVolumeTable_removeEntry(entry);
         }
         entry = next;
     }

     /* Update cache timestamp */
     gettimeofday(&vollist_cache_timestamp, NULL);
     
     /* Clean up */
     free (data);
     data = NULL;
     data_len = 0;
 }

void update_vollist_if_necessry()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec - vollist_cache_timestamp.tv_sec > MAX_CACHE_AGE)
  { update_vollist(); }
}

void update_volume(struct xsanVolumeTable_entry *entry)
{
  struct timeval now;
  gettimeofday (&now, NULL);

  char *command_str;
  asprintf (&command_str, "echo 'select %s'; show long", entry->xsanVolumeName);
  // char *data = x_command_run(command_str, 0);
  free (command_str);
  // if (!data) return;
  // size_t data_len = strlen(data);

  int fd;
  fd = open ("show_long_example_AM_01.txt", O_RDONLY);
  char *data = malloc (65536);
  size_t data_len =  read (fd, data, 65536);
  data[data_len] = '\0';     
  
  extract_string_from_regex (data, data_len, "^[ ]*Created[ ]*:[ \\t]+(.*)$", &entry->xsanVolumeCreated, &entry->xsanVolumeCreated_len);
  entry->xsanVolumeActiveConnections = extract_uint_from_regex (data, data_len, "^[ ]*Active Connections[ ]*:[ \\t]+(.*)$");
  entry->xsanVolumeFsBlockSize = extract_uint_from_regex (data, data_len, "^[ ]*Fs Block Size[ ]*:[ \\t]+(.*)$");
  entry->xsanVolumeMsgBufferSize = extract_uint_from_regex (data, data_len, "^[ ]*Msg Buffer Size[ ]*:[ \\t]+(.*)$");
  entry->xsanVolumeDiskDevices = extract_uint_from_regex (data, data_len, "^[ ]*Disk Devices[ ]*:[ \\t]+(.*)$");
  entry->xsanVolumeStripeGroups = extract_uint_from_regex (data, data_len, "^[ ]*Stripe Groups[ ]*:[ \\t]+(.*)$");
  extract_U64_from_regex (data, data_len, "^[ ]*Fs Blocks[ ]*:[ \\t]+(\\d*).*$", &entry->xsanVolumeTotalBlocks);
  extract_U64_from_regex (data, data_len, "^[ ]*Fs Blocks Free[ ]*:[ \\t]+(\\d*).*$", &entry->xsanVolumeFreeBlocks);  
  entry->xsanVolumeTotalKBlocks = scale_U64_to_K (&entry->xsanVolumeTotalBlocks);
  entry->xsanVolumeFreeKBlocks = scale_U64_to_K (&entry->xsanVolumeFreeBlocks);
  entry->xsanVolumeUtilization = 100 - extract_uint_from_regex (data, data_len, "^[ ]*Fs Blocks Free[ ]*:[ \\t]+\\d+ \\(.*\\) \\((\\d+)%\\)$");
  
  update_stripegroups (data, data_len, entry->xsanVolumeIndex);
  free (data);
  data = NULL;
  data_len = 0;

  /* Update cache timestamp */
  gettimeofday(&entry->volume_cache_timestamp, NULL);  
}

void update_volume_if_necessary(struct xsanVolumeTable_entry *entry)
{
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec - entry->volume_cache_timestamp.tv_sec > MAX_CACHE_AGE)
  { update_volume (entry); }
}

/** handles requests for the xsanVolumeTable table */
int
xsanVolumeTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request;
    netsnmp_table_request_info *table_info;
    struct xsanVolumeTable_entry          *table_entry;

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
    
        update_vollist_if_necessry();
        for (request=requests; request; request=request->next) {
            table_entry = (struct xsanVolumeTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
            
            if (table_entry) update_volume_if_necessary(table_entry);
    
            switch (table_info->colnum) {
            case COLUMN_XSANVOLUMEINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->xsanVolumeIndex);
                break;
            case COLUMN_XSANVOLUMENAME:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeName,
                                          table_entry->xsanVolumeName_len);
                break;
            case COLUMN_XSANVOLUMEFSSINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->xsanVolumeFSSIndex);
                break;
            case COLUMN_XSANVOLUMESTATE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->xsanVolumeState);
                break;
            case COLUMN_XSANVOLUMESTATEMESSAGE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeStateMessage,
                                          table_entry->xsanVolumeStateMessage_len);
                break;
            case COLUMN_XSANVOLUMELASTADMINACTION:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeLastAdminAction,
                                          table_entry->xsanVolumeLastAdminAction_len);
                break;
            case COLUMN_XSANVOLUMELASTTERMINATION:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeLastTermination,
                                          table_entry->xsanVolumeLastTermination_len);
                break;
            case COLUMN_XSANVOLUMELAUNCHCOUNT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->xsanVolumeLaunchCount);
                break;
            case COLUMN_XSANVOLUMECOREDUMPCOUNT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                            table_entry->xsanVolumeCoreDumpCount);
                break;
            case COLUMN_XSANVOLUMEFLAGS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeFlags,
                                          table_entry->xsanVolumeFlags_len);
                break;
            case COLUMN_XSANVOLUMECREATED:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->xsanVolumeCreated,
                                          table_entry->xsanVolumeCreated_len);
                break;
            case COLUMN_XSANVOLUMEACTIVECONNECTIONS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeActiveConnections);
                break;
            case COLUMN_XSANVOLUMEFSBLOCKSIZE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeFsBlockSize);
                break;
            case COLUMN_XSANVOLUMEMSGBUFFERSIZE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeMsgBufferSize);
                break;
            case COLUMN_XSANVOLUMEDISKDEVICES:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeDiskDevices);
                break;
            case COLUMN_XSANVOLUMESTRIPEGROUPS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeStripeGroups);
                break;
            case COLUMN_XSANVOLUMETOTALKBLOCKS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeTotalKBlocks);
                break;
            case COLUMN_XSANVOLUMEFREEKBLOCKS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeFreeKBlocks);
                break;
            case COLUMN_XSANVOLUMETOTALBLOCKS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_COUNTER64,
                                            (u_char*)&table_entry->xsanVolumeTotalBlocks, sizeof(U64));
                break;
            case COLUMN_XSANVOLUMEFREEBLOCKS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_COUNTER64,
                                            (u_char *)&table_entry->xsanVolumeFreeBlocks, sizeof(U64));
                break;
            case COLUMN_XSANVOLUMEUTILIZATION:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumeUtilization);
                break;
            case COLUMN_XSANVOLUMEPID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumePid);
                break;
            case COLUMN_XSANVOLUMEPORT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->xsanVolumePort);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        break;

    }
    return SNMP_ERR_NOERROR;
}
