/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 15999 2007-03-25 22:32:02Z dts12 $
 */
#ifndef XSANAFFINITYTABLE_H
#define XSANAFFINITYTABLE_H

/* function declarations */
void init_xsanAffinityTable(void);
void initialize_table_xsanAffinityTable(void);
Netsnmp_Node_Handler xsanAffinityTable_handler;
Netsnmp_First_Data_Point  xsanAffinityTable_get_first_data_point;
Netsnmp_Next_Data_Point   xsanAffinityTable_get_next_data_point;

/* column number definitions for table xsanAffinityTable */
       #define COLUMN_XSANVOLUMEINDEX		1
       #define COLUMN_XSANAFFINITYINDEX		2
       #define COLUMN_XSANAFFINITYNAME		3
       #define COLUMN_XSANAFFINITYSTRIPEGROUPS		4
       #define COLUMN_XSANAFFINITYTOTALKBLOCKS		5
       #define COLUMN_XSANAFFINITYFREEKBLOCKS		6
       #define COLUMN_XSANAFFINITYTOTALBLOCKS		7
       #define COLUMN_XSANAFFINITYFREEBLOCKS		8
       #define COLUMN_XSANAFFINITYUTILIZATION		9
#endif /* XSANAFFINITYTABLE_H */

long find_or_create_affinity_index (long volumeIndex, char *affinityKey);
