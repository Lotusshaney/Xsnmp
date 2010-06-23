/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 15999 2007-03-25 22:32:02Z dts12 $
 */
#ifndef XSANNODETABLE_H
#define XSANNODETABLE_H

/* function declarations */
void init_xsanNodeTable(void);
void initialize_table_xsanNodeTable(void);
Netsnmp_Node_Handler xsanNodeTable_handler;
Netsnmp_First_Data_Point  xsanNodeTable_get_first_data_point;
Netsnmp_Next_Data_Point   xsanNodeTable_get_next_data_point;

/* column number definitions for table xsanNodeTable */
       #define COLUMN_XSANVOLUMEINDEX		1
       #define COLUMN_XSANSTRIPEGROUPINDEX		2
       #define COLUMN_XSANNODEINDEX		3
       #define COLUMN_XSANNODENAME		4
#endif /* XSANNODETABLE_H */

void update_nodes(char *data, size_t data_len, long volumeIndex, long stripeGroupIndex);
