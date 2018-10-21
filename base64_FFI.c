#include "base64_FFI.h"

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#define BUF_MAX 10240
#define BUF_INIT 1024

uw_Basis_string uw_Base64_FFI_encode(uw_context ctx, uw_Basis_blob blob)
{
  const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  const uint8_t *data = (const uint8_t *)blob.data;
  //size_t resultIndex = 0;
  size_t x;
  uint32_t n = 0;
  size_t dataLength = blob.size;
  int padCount = dataLength % 3;
  uint8_t n0, n1, n2, n3;
  
  //size_t resultSize = 128;
  //char* result = malloc(sizeof(char) * resultSize);

  uw_buffer *buf = uw_malloc(ctx, sizeof(uw_buffer));
  uw_buffer_init(BUF_MAX, buf, BUF_INIT);
  uw_push_cleanup(ctx, (void (*)(void*)) uw_buffer_free, buf);
  
  /* increment over the length of the string, three characters at a time */
  for (x = 0; x < dataLength; x += 3) 
    {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+1) < dataLength)
	n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+2) < dataLength)
	n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      //      if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
	//	{
	  // TODO realloc and continue
	  //	  uw_error(ctx, FATAL, "base64 encoding: not enough memory");
	  //	}
      //result[resultIndex++] = base64chars[n0];
      uw_buffer_append(buf, uw_Basis_str1(ctx, base64chars[n0]), 1);
	    
      //      if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
      //	{
	  // TODO realloc and continue
      //	  uw_error(ctx, FATAL, "base64 encoding: not enough memory");
      //	}
      //result[resultIndex++] = base64chars[n1];
      uw_buffer_append(buf, uw_Basis_str1(ctx, base64chars[n1]), 1);

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
	{
	  //	  if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
	    //	    {
	      // TODO realloc and continue
	      //	      uw_error(ctx, FATAL, "base64 encoding: not enough memory");
	      //	    }
	  //result[resultIndex++] = base64chars[n2];
	  uw_buffer_append(buf, uw_Basis_str1(ctx, base64chars[n2]), 1);
	}

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
	{
	  //	  if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
	  //	    {
	      // TODO realloc and continue
	  //	      uw_error(ctx, FATAL, "base64 encoding: not enough memory");
	  //	    }
	  //result[resultIndex++] = base64chars[n3];
	  uw_buffer_append(buf, uw_Basis_str1(ctx, base64chars[n3]), 1);
	}
    }

  /*
   * create and add padding that is required if we did not have a multiple of 3
   * number of characters available
   */
  if (padCount > 0) 
    { 
      for (; padCount < 3; padCount++) 
	{ 
	  //	  if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
	    //	    {
	      // TODO realloc and continue
	      //	      uw_error(ctx, FATAL, "base64 encoding: not enough memory");
	      //	    }
	  //result[resultIndex++] = '=';
	  uw_buffer_append(buf, uw_Basis_str1(ctx, '='), 1);
	} 
    }
  //  if(resultIndex >= resultSize) // return 1;   /* indicate failure: buffer too small */
  //    {
      // TODO realloc and continue
  //      uw_error(ctx, FATAL, "base64 encoding: not enough memory");
      
  //    }
  //result[resultIndex] = 0;
  uw_buffer_append(buf, uw_Basis_str1(ctx, '\0'), 1);
  
  //fprintf(stderr, "size: %d\n", (int)resultIndex);
  //fprintf(stderr, "generated: %s \n", result);
  
  
  // TODO copy the buffer to a uw_malloced string.
  // TODO free the buffer

  char* ret = uw_strdup(ctx, buf->start);
  //free(result);
  uw_pop_cleanup(ctx);
  return ret;
}

