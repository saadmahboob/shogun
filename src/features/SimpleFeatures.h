#ifndef _SIMPLEFEATURES__H__
#define _SIMPLEFEATURES__H__

#include "lib/common.h"
#include "lib/Mathmatics.h"
#include "lib/Cache.h"
#include "preproc/SimplePreProc.h"
#include "lib/io.h"
#include "lib/Cache.h"

#include <string.h>
#include <assert.h>

#include "features/Features.h"

template <class ST> class CSimpleFeatures: public CFeatures
{
 public:
	 CSimpleFeatures(long size) : CFeatures(size), num_vectors(0), num_features(0), feature_matrix(NULL), feature_cache(NULL)
	 {
	 }

	 CSimpleFeatures(const CSimpleFeatures & orig) : CFeatures(orig), 
	 num_vectors(orig.num_vectors), num_features(orig.num_features)
	 {
		 if (orig.feature_matrix)
		 {
			 feature_matrix=new ST(num_vectors*num_features);
			 memcpy(feature_matrix, orig.feature_matrix, sizeof(double)*num_vectors*num_features); 
		 }
	 }

	CSimpleFeatures(char* fname) : CFeatures(fname)
	{
	}

	 virtual ~CSimpleFeatures()
	 {
		 delete[] feature_matrix;
		 delete feature_cache;
	 }
  
  /** get feature vector for sample num
      from the matrix as it is if matrix is
      initialized, else return
      preprocessed compute_feature_vector  
      @param num index of feature vector
      @param len length is returned by reference
  */
  ST* get_feature_vector(long num, long& len, bool& free)
  {
	  len=num_features; 
	  assert(num<num_vectors);

	  if (feature_matrix)
	  {
		  //      CIO::message("returning %i th column of feature_matrix\n", (int)num) ;
		  free=false ;
		  return &feature_matrix[num*num_features];
	  } 
	  else
	  {
		  //CIO::message("computing %i th feature vector\n", (int)num) ;

		  ST* feat=NULL;
		  free=false;

		  if (feature_cache)
		  {
			  feat=feature_cache->lock_entry(num);

			  if (feat)
				  return feat;
			  else
			  {
				  feat=feature_cache->set_entry(num);
			  }
		  }

		  if (!feat)
			  free=true;
		  feat=compute_feature_vector(num, len, feat);


		  if (get_num_preproc())
		  {
			  //CIO::message("preprocessing %i th feature vector\n", (int)num) ;

			  int tmp_len=len;
			  ST* tmp_feat_before = feat;
			  ST* tmp_feat_after = NULL;

			  for (int i=0; i<get_num_preproc(); i++)
			  {
				  tmp_feat_after=((CSimplePreProc<ST>*) get_preproc(i))->apply_to_feature_vector(tmp_feat_before, tmp_len);

				  if (i!=0)	// delete feature vector, except for the the first one, i.e., feat
					  delete[] tmp_feat_before;
				  tmp_feat_before=tmp_feat_after;
			  }

			  memcpy(feat, tmp_feat_after, sizeof(ST)*tmp_len);
			  delete[] tmp_feat_after;
			  //len=num_features=len2 ;
			  len=tmp_len ;
			  CIO::message(stderr, "len: %d len2: %d\n", len, num_features);
		  }
		  return feat ;
	  }
  }

  void free_feature_vector(ST* feat_vec, int num, bool free)
  {
	  if (feature_cache)
		  feature_cache->unlock_entry(num);

	  if (free)
		  delete[] feat_vec ;
  } 
  
  /// get the pointer to the feature matrix
  /// num_feat,num_vectors are returned by reference
  ST* get_feature_matrix(long &num_feat, long &num_vec)
  {
	  num_feat=num_features;
	  num_vec=num_vectors;
	  return feature_matrix;
  }
  
  /** set feature matrix
      necessary to set feature_matrix, num_features, num_vectors, where
      num_features is the column offset, and columns are linear in memory
      see below for definition of feature_matrix
  */
  virtual void set_feature_matrix(ST* fm, long num_feat, long num_vec)
  {
	  feature_matrix=fm;
	  num_features=num_feat;
	  num_vectors=num_vec;
  }

  virtual bool preproc_feature_matrix(bool force_preprocessing=false)
  {
	CIO::message("preprocd: %d, force: %d\n", preprocessed, force_preprocessing);

	if ( feature_matrix && get_num_preproc() && (!preprocessed || force_preprocessing) )
	{
	    preprocessed=true;	

		for (int i=0; i<get_num_preproc(); i++)
		{
			CIO::message("preprocessing using preproc %s\n", get_preproc(i)->get_name());
			if (((CSimplePreProc<ST>*) get_preproc(i))->apply_to_feature_matrix(this) == NULL)
				return false;
		}
		return true;
	}
	else
	{
		CIO::message("no feature matrix available or features already preprocessed - skipping.\n");
		return false;
	}
  }

  virtual int get_size() { return sizeof(ST); }
  virtual inline long  get_num_vectors() { return num_vectors; }
  inline long  get_num_features() { return num_features; }
  inline void set_num_features(int num)
  { 
	  num_features= num; 

	  if (num_features && num_vectors)
	  {
		  delete feature_cache;
		  feature_cache= new CCache<ST>(get_cache_size(), num_features, num_vectors);
	  }
  }

  inline void set_num_vectors(int num)
  {
	  num_vectors= num;
	  if (num_features && num_vectors)
	  {
		  delete feature_cache;
		  feature_cache= new CCache<ST>(get_cache_size(), num_features, num_vectors);
	  }
  }
	
protected:
  /// compute feature vector for sample num
  /// if target is set the vector is written to target
  /// len is returned by reference
  virtual ST* compute_feature_vector(long num, long& len, ST* target=NULL)
  {
	  len=0;
	  return NULL;
  }

  /// number of vectors in cache
  long num_vectors;
 
  /// number of features in cache
  long num_features;
  
  ST* feature_matrix;
  CCache<ST>* feature_cache;
};
#endif
