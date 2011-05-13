/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2010 Soeren Sonnenburg
 * Copyright (C) 2010 Berlin Institute of Technology
 */
#ifndef __SERIALIZABLE_ASCII_READER_00_H__
#define __SERIALIZABLE_ASCII_READER_00_H__

#include "lib/SerializableAsciiFile.h"

namespace shogun
{
class SerializableAsciiReader00
	:public CSerializableFile::TSerializableReader {
	CSerializableAsciiFile* m_file;

public:
	explicit SerializableAsciiReader00(CSerializableAsciiFile* file);
	virtual ~SerializableAsciiReader00(void);

	/** @return object name */
	inline virtual const char* get_name(void) const {
		return "SerializableAsciiReader00";
	}

	virtual bool read_scalar_wrapped(
		const TSGDataType* type, void* param);

	virtual bool read_cont_begin_wrapped(
		const TSGDataType* type, index_t* len_read_y,
		index_t* len_read_x);
	virtual bool read_cont_end_wrapped(
		const TSGDataType* type, index_t len_read_y,
		index_t len_read_x);

	virtual bool read_string_begin_wrapped(
		const TSGDataType* type, index_t* length);
	virtual bool read_string_end_wrapped(
		const TSGDataType* type, index_t length);

	virtual bool read_stringentry_begin_wrapped(
		const TSGDataType* type, index_t y);
	virtual bool read_stringentry_end_wrapped(
		const TSGDataType* type, index_t y);

	virtual bool read_sparse_begin_wrapped(
		const TSGDataType* type, index_t* vec_index,
		index_t* length);
	virtual bool read_sparse_end_wrapped(
		const TSGDataType* type, index_t* vec_index,
		index_t length);

	virtual bool read_sparseentry_begin_wrapped(
		const TSGDataType* type, SGSparseMatrixEntry<char>* first_entry,
		index_t* feat_index, index_t y);
	virtual bool read_sparseentry_end_wrapped(
		const TSGDataType* type, SGSparseMatrixEntry<char>* first_entry,
		index_t* feat_index, index_t y);

	virtual bool read_item_begin_wrapped(
		const TSGDataType* type, index_t y, index_t x);
	virtual bool read_item_end_wrapped(
		const TSGDataType* type, index_t y, index_t x);

	virtual bool read_sgserializable_begin_wrapped(
		const TSGDataType* type, char* sgserializable_name,
		EPrimitiveType* generic);
	virtual bool read_sgserializable_end_wrapped(
		const TSGDataType* type, const char* sgserializable_name,
		EPrimitiveType generic);

	virtual bool read_type_begin_wrapped(
		const TSGDataType* type, const char* name,
		const char* prefix);
	virtual bool read_type_end_wrapped(
		const TSGDataType* type, const char* name,
		const char* prefix);
};
}

#endif /* __SERIALIZABLE_ASCII_READER_00_H__  */
