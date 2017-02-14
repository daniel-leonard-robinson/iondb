#include <stdio.h>
#include "linear_hash_types.h"

linear_hash_table_t *
linear_hash_init(
	int					initial_size,
	int					split_threshold,
	array_list_t		*bucket_map,
	linear_hash_table_t *linear_hash
);

void
linear_hash_increment_num_records(
	linear_hash_table_t *linear_hash
);

/* decrement the count of the records stored in the linear hash */
void
linear_hash_decrement_num_records(
	linear_hash_table_t *linear_hash
);

void
linear_hash_increment_num_buckets(
	linear_hash_table_t *linear_hash
);

void
linear_hash_update_state(
	linear_hash_table_t *linear_hash
);

int
linear_hash_bucket_is_full(
	linear_hash_bucket_t bucket
);

/* split function */
void
split(
	linear_hash_table_t *linear_hash
);

int
linear_hash_insert(
	int					id,
	int					hash_bucket_idx,
	linear_hash_table_t *linear_hash
);

/* linear hash operations */
linear_hash_record_t
linear_hash_get(
	int					id,
	linear_hash_table_t *linear_hash
);

void
linear_hash_delete(
	int					id,
	linear_hash_table_t *linear_hash
);

linear_hash_record_t
linear_hash_get_record(
	file_offset loc
);

void
linear_hash_write_record(
	file_offset				record_loc,
	linear_hash_record_t	record,
	linear_hash_table_t		*linear_hash
);

/* check if linear hash is above its split threshold */
int
linear_hash_above_threshold(
	linear_hash_table_t *linear_hash
);

/* BUCKET OPERATIONS */
void
write_new_bucket(
	int					idx,
	linear_hash_table_t *linear_hash
);

/* returns the struct representing the bucket at the specified index */
linear_hash_bucket_t
linear_hash_get_bucket(
	file_offset			bucket_loc,
	linear_hash_table_t *linear_hash
);

void
linear_hash_update_bucket(
	file_offset				bucket_loc,
	linear_hash_bucket_t	bucket,
	linear_hash_table_t		*linear_hash
);

file_offset
create_overflow_bucket(
	int					bucket_idx,
	linear_hash_table_t *linear_hash
);

file_offset
get_bucket_records_location(
	file_offset bucket_loc
);

/* hash methods */
int
hash_to_bucket(
	int					id,
	linear_hash_table_t *linear_hash
);

int
insert_hash_to_bucket(
	int					id,
	linear_hash_table_t *linear_hash
);

/* returns the struct representing the bucket at the specified index */
linear_hash_bucket_t
linear_hash_get_overflow_bucket(
	file_offset loc
);

/* Returns the file offset where bucket with index idx begins */
file_offset
bucket_idx_to_file_offset(
	int					idx,
	linear_hash_table_t *linear_hash
);

void
linear_hash_increment_next_split(
	linear_hash_table_t *linear_hash
);

void
print_linear_hash_bucket(
	linear_hash_bucket_t bucket
);

void
print_linear_hash_bucket_map(
	linear_hash_table_t *linear_hash
);

/* Write the offset of bucket idx to the map in linear hash state */
void
store_bucket_loc_in_map(
	int			idx,
	file_offset bucket_loc
);

/* ARRAY_LIST METHODS */
array_list_t *
array_list_init(
	int				init_size,
	array_list_t	*array_list
);

void
array_list_insert(
	int				bucket_idx,
	file_offset		bucket_loc,
	array_list_t	*array_list
);

file_offset
array_list_get(
	int				bucket_idx,
	array_list_t	*array_list
);

/* DEBUG METHODS */
void
print_array_list_data(
	array_list_t *array_list
);

void
print_all_linear_hash_index_buckets(
	int					idx,
	linear_hash_table_t *linear_hash
);

void
print_linear_hash_record(
	linear_hash_record_t record
);

void
print_linear_hash_bucket_from_idx(
	int					idx,
	linear_hash_table_t *linear_hash
);

void
print_linear_hash_state(
	linear_hash_table_t *linear_hash
);