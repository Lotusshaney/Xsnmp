/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 15999 2007-03-25 22:32:02Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <pcre.h>
#include "fsTable.h"
#include "log.h"
#include "command.h"
#include "util.h"

#define OVECCOUNT 30

/** Initializes the fsTable module */
void
init_fsTable(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_fsTable();
}

static struct timeval volume_cache_timestamp = { 0, 0 };
static int last_index_used = 0;

/** Initialize the fsTable table by defining its contents and how it's structured */
void
initialize_table_fsTable(void)
{
    static oid fsTable_oid[] = {1,3,6,1,4,1,20038,2,1,4,1};
    size_t fsTable_oid_len   = OID_LENGTH(fsTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    reg = netsnmp_create_handler_registration(
              "fsTable",     fsTable_handler,
              fsTable_oid, fsTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: fsIndex */
                           0);
    table_info->min_column = COLUMN_FSINDEX;
    table_info->max_column = COLUMN_FSSMARTMESSAGE;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = fsTable_get_first_data_point;
    iinfo->get_next_data_point  = fsTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

    /* Typical data structure for a row entry */
struct fsTable_entry {
    /* Index values */
    long fsIndex;

    /* Column values */
    char *fsFilesystem;
    size_t fsFilesystem_len;
    char *fsMountpoint;
    size_t fsMountpoint_len;
    u_long fsSize;
    u_long fsUsed;
    u_long fsAvail;
    u_long fsUtilization;
    long fsWriteable;
    long fsRemovable;
    long fsBootable;
    long fsSmartStatus;
    char *fsSmartMessage;
    size_t fsSmartMessage_len;
    
    /* Obsolescence checking */
    time_t last_seen;

    /* Illustrate using a simple linked list */
    int   valid;
    struct fsTable_entry *next;
};

struct fsTable_entry  *fsTable_head;

/* create a new row in the (unsorted) table */
struct fsTable_entry *
fsTable_createEntry(
                 long  fsIndex) {
    struct fsTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct fsTable_entry);
    if (!entry)
        return NULL;

    entry->fsIndex = fsIndex;
    entry->next = fsTable_head;
    entry->fsWriteable = -1;
    entry->fsRemovable = -1;
    entry->fsBootable = -1;
    entry->fsSmartStatus = -1;
    fsTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
fsTable_removeEntry( struct fsTable_entry *entry ) {
    struct fsTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = fsTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        fsTable_head = ptr->next;
    else
        prev->next = ptr->next;
        
    if (entry->fsFilesystem) free (entry->fsFilesystem);
    if (entry->fsMountpoint) free (entry->fsMountpoint);

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
fsTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    *my_loop_context = fsTable_head;
    return fsTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
fsTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct fsTable_entry *entry = (struct fsTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->fsIndex );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}

void update_volume_disk (struct fsTable_entry *entry)
{
  /* Uses 'diskutil' to update the disk referenced in disk_name */
  char *command_str;
  asprintf (&command_str, "diskutil info %s", entry->fsFilesystem);
  char *data = x_command_run (command_str, 0);
  free(command_str);
  size_t data_len = strlen(data);
  
  entry->fsWriteable = !extract_boolean_from_regex(data, data_len, "Read-Only Volume:[ ]+(\\w+)$");
  entry->fsRemovable = extract_boolean_from_regex(data, data_len, "Ejectable:[ ]+(\\w+)$");
  
  char *boot_str = NULL;
  size_t boot_str_len = 0;
  if (extract_string_from_regex(data, data_len, "Bootable:[ ]+(.*)$", &boot_str, &boot_str_len))
  {
    if (!strcmp(boot_str, "Is bootable"))
    {
      entry->fsBootable = 1;
    }
    else if (!strcmp(boot_str, "Not bootable"))
    {
      entry->fsBootable = 0;
    }
  }
  if (boot_str) free (boot_str);
  
  if (extract_string_from_regex(data, data_len, "SMART Status:[ ]+(.*)$", &entry->fsSmartMessage, &entry->fsSmartMessage_len))
  { 
    if (!strcmp(entry->fsSmartMessage, "Verified")) entry->fsSmartStatus = 1;
    else if (!strcmp(entry->fsSmartMessage, "Not Supported")) entry->fsSmartStatus = 0;
    else entry->fsSmartStatus = 2;
  } 

  free (data);
}

void update_volumes()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  
  char *data = x_command_run("df -m", 0);
  if (!data) return;
  size_t data_len = strlen(data);

  const char *error;
  int erroffset;
  int ovector[OVECCOUNT];
  pcre *re = pcre_compile("^(.*)[ ]+(\\d+)[ ]+(\\d+)[ ]+(\\d+)[ ]+(\\d+)%[ ]+(.*)$", PCRE_MULTILINE, &error, &erroffset, NULL);
  if (re == NULL) { x_printf ("update_ram failed to compile regex for wired"); free (data); return; }

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

    if (rc == PCRE_ERROR_NOMATCH)
    {
      if (options == 0) break;
      ovector[1] = start_offset + 1;
      continue;    /* Go round the loop again */
    }

    /* Other matching errors are not recoverable. */
    if (rc > 0)
    {
      /* Matched a volume 
       * 0=FullString 1(2)=Disk 2(4)=Size(blocks) 3(6)=Used 4(8)=Avail 5(10)=Percent 6(12)=Volume
       */
      char *volname_str;
      asprintf (&volname_str, "%.*s", ovector[13] - ovector[12], data + ovector[12]);
      trim_end(volname_str);
      
      struct fsTable_entry *entry = fsTable_head;
      while (entry)
      {
        if (!strcmp(volname_str, entry->fsMountpoint)) break;
        entry = entry->next;
      }
      if (!entry)
      {
        last_index_used++;
        entry = fsTable_createEntry(last_index_used);
        entry->fsMountpoint = strdup(volname_str);
        entry->fsMountpoint_len = strlen (entry->fsMountpoint);
      }
      entry->last_seen = now.tv_sec;
      free (volname_str);
      volname_str = NULL;
      if (entry->fsFilesystem) free (entry->fsFilesystem);
      asprintf (&entry->fsFilesystem, "%.*s", ovector[3] - ovector[2], data + ovector[2]);
      trim_end(entry->fsFilesystem);
      entry->fsFilesystem_len = strlen (entry->fsFilesystem);
      entry->fsSize = extract_uint_in_range(data + ovector[4], ovector[5] - ovector[4]);
      entry->fsUsed = extract_uint_in_range(data + ovector[6], ovector[7] - ovector[6]);
      entry->fsAvail = extract_uint_in_range(data + ovector[8], ovector[9] - ovector[8]);
      entry->fsUtilization = extract_uint_in_range(data + ovector[10], ovector[11] - ovector[10]);
      
      /* Update extra info on the disk from diskutil */
      if (entry->fsFilesystem && strstr(entry->fsFilesystem, "/dev/")) update_volume_disk(entry);
    }
    else
    {
      x_printf("Matching error %d\n", rc);
      pcre_free(re);    /* Release memory used for the compiled pattern */
      return;
    }  
  }
  
  /* Clean up */
  pcre_free(re);
  
  /* Check for obsolete entries */
  struct fsTable_entry *entry = fsTable_head;
  while (entry)
  {
    struct fsTable_entry *next = entry->next;
    if (entry->last_seen != now.tv_sec)
    {
      /* Entry is obsolete */
      fsTable_removeEntry(entry);
    }
    entry = next;
  }

  /* Update cache timestamp */
  gettimeofday(&volume_cache_timestamp, NULL);    
  
  /* Clean up */
  free (data);
  data = NULL;
  data_len = 0;
}

#define MAX_CACHE_TIMEOUT 10

void update_volumes_if_necessary()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  if ((now.tv_sec - volume_cache_timestamp.tv_sec) > MAX_CACHE_TIMEOUT)
  { update_volumes(); }
}

/** handles requests for the fsTable table */
int
fsTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request;
    netsnmp_table_request_info *table_info;
    struct fsTable_entry          *table_entry;

    update_volumes_if_necessary();

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {
            table_entry = (struct fsTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_FSINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fsIndex);
                break;
            case COLUMN_FSFILESYSTEM:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->fsFilesystem,
                                          table_entry->fsFilesystem_len);
                break;
            case COLUMN_FSMOUNTPOINT:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->fsMountpoint,
                                          table_entry->fsMountpoint_len);
                break;
            case COLUMN_FSSIZE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->fsSize);
                break;
            case COLUMN_FSUSED:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->fsUsed);
                break;
            case COLUMN_FSAVAIL:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->fsAvail);
                break;
            case COLUMN_FSUTILIZATION:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_GAUGE,
                                            table_entry->fsUtilization);
                break;
            case COLUMN_FSWRITEABLE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fsWriteable);
                break;
            case COLUMN_FSREMOVABLE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fsRemovable);
                break;
            case COLUMN_FSBOOTABLE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fsBootable);
                break;
            case COLUMN_FSSMARTSTATUS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->fsSmartStatus);
                break;
            case COLUMN_FSSMARTMESSAGE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                 (u_char*)table_entry->fsSmartMessage,
                                          table_entry->fsSmartMessage_len);
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
