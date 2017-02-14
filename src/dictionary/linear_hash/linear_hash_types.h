/*  */
/* Created by Spencer MacBeth on 2017-02-12. */
/*  */
#include <stdio.h>
#include "../../key_value/kv_system.h"
#include "../dictionary.h"

typedef long file_offset;

/* SIMPLE ARRAY_LIST FOR BUCKET MAP */
typedef struct {
	int			current_size;
	file_offset *data;
} array_list_t;

/* definition of linear hash record, with a type and pointer instance declared for later use */
typedef struct {
	int			id;
	file_offset next;
	int			value;
} linear_hash_record_t;

/* buckets */
typedef struct {
	int			idx;
	int			record_count;
	file_offset anchor_record;
	file_offset overflow_location;
} linear_hash_bucket_t;

/* function pointer syntax: return_type (*function_name) (arg_type) */
/* linear hash structure definition, with a type and pointer instance declared for later use */
typedef struct {
	/**> Parent structure that holds dictionary level information. */
	ion_dictionary_parent_t super;

	int						initial_size;
	int						next_split;
	int						split_threshold;
	int						num_buckets;
	int						num_records;
	int						records_per_bucket;
	FILE					*database;

	/* points to the current location in the data */
	file_offset				data_pointer;

	/* maps the location of the head of the linked list of buckets corresponding to its index */
	array_list_t			*bucket_map;
} linear_hash_table_t;