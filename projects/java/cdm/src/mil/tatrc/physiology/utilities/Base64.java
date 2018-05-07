/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

package mil.tatrc.physiology.utilities;
import java.io.*;
 
 
 /**
  * This class provides encode/decode for RFC 2045 Base64 as defined by RFC 2045,
  * N. Freed and N. Borenstein. RFC 2045: Multipurpose Internet Mail Extensions
  * (MIME) Part One: Format of Internet Message Bodies. Reference 1996 Available
  * at: http://www.ietf.org/rfc/rfc2045.txt 
  * 
  * Data can be enoded/decoded from either an in-memory array or streams can be used.
  * 
  * 
  * 
  */
 public final class Base64
 {
 
 
   /**
    * Encodes hex octects into Base64
    * 
    * @param binaryData Array containing binaryData
    * @return Encoded Base64 array
    */
   public static String encode(byte[] binaryData)
   {
 
     if (binaryData == null) return null;
 
     int lengthDataBits = binaryData.length * EIGHTBIT;
     if (lengthDataBits == 0){
       return "";
     }
 
     int fewerThan24bits = lengthDataBits % TWENTYFOURBITGROUP;
     int numberTriplets = lengthDataBits / TWENTYFOURBITGROUP;
     int numberQuartet = fewerThan24bits != 0 ? numberTriplets + 1 : numberTriplets;
     int numberLines = (numberQuartet - 1) / 19 + 1;
     char encodedData[] = null;
 
     encodedData = new char[numberQuartet * 4 + numberLines];
 
     byte k = 0, l = 0, b1 = 0, b2 = 0, b3 = 0;
 
     int encodedIndex = 0;
     int dataIndex = 0;
     int i = 0;
 
     for (int line = 0; line < numberLines - 1; line++)  {
       for (int quartet = 0; quartet < 19; quartet++)  {
         b1 = binaryData[dataIndex++];
         b2 = binaryData[dataIndex++];
         b3 = binaryData[dataIndex++];
 
         l = (byte) (b2 & 0x0f);
         k = (byte) (b1 & 0x03);
 
         byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2) : (byte) ((b1) >> 2 ^ 0xc0);
         byte val2 = ((b2 & SIGN) == 0) ? (byte) (b2 >> 4) : (byte) ((b2) >> 4 ^ 0xf0);
         byte val3 = ((b3 & SIGN) == 0) ? (byte) (b3 >> 6) : (byte) ((b3) >> 6 ^ 0xfc);
 
         encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
         encodedData[encodedIndex++] = lookUpBase64Alphabet[val2 | (k << 4)];
         encodedData[encodedIndex++] = lookUpBase64Alphabet[(l << 2) | val3];
         encodedData[encodedIndex++] = lookUpBase64Alphabet[b3 & 0x3f];
         i++;
       }
       encodedData[encodedIndex++] = 0xa;
     }
 
     for (; i < numberTriplets; i++) {
       b1 = binaryData[dataIndex++];
       b2 = binaryData[dataIndex++];
       b3 = binaryData[dataIndex++];
 
       l = (byte) (b2 & 0x0f);
       k = (byte) (b1 & 0x03);
 
       byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2) : (byte) ((b1) >> 2 ^ 0xc0);
       byte val2 = ((b2 & SIGN) == 0) ? (byte) (b2 >> 4) : (byte) ((b2) >> 4 ^ 0xf0);
       byte val3 = ((b3 & SIGN) == 0) ? (byte) (b3 >> 6) : (byte) ((b3) >> 6 ^ 0xfc);
 
       encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[val2 | (k << 4)];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[(l << 2) | val3];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[b3 & 0x3f];
     }
 
     // form integral number of 6-bit groups
     if (fewerThan24bits == EIGHTBIT)  {
       b1 = binaryData[dataIndex];
       k = (byte) (b1 & 0x03);
       byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2) : (byte) ((b1) >> 2 ^ 0xc0);
       encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[k << 4];
       encodedData[encodedIndex++] = PAD;
       encodedData[encodedIndex++] = PAD;
     }
     else if (fewerThan24bits == SIXTEENBIT)  {
       b1 = binaryData[dataIndex];
       b2 = binaryData[dataIndex + 1];
       l = (byte) (b2 & 0x0f);
       k = (byte) (b1 & 0x03);
 
       byte val1 = ((b1 & SIGN) == 0) ? (byte) (b1 >> 2) : (byte) ((b1) >> 2 ^ 0xc0);
       byte val2 = ((b2 & SIGN) == 0) ? (byte) (b2 >> 4) : (byte) ((b2) >> 4 ^ 0xf0);
 
       encodedData[encodedIndex++] = lookUpBase64Alphabet[val1];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[val2 | (k << 4)];
       encodedData[encodedIndex++] = lookUpBase64Alphabet[l << 2];
       encodedData[encodedIndex++] = PAD;
     }
      encodedData[encodedIndex] = 0xa;
 
     return new String(encodedData);
   }
 
   /**
    * Decodes Base64 data into octects
    * 
    * @param encoded String encoded in Base64
    * @return Byte array containing decoded data.
    */
   public static byte[] decode(String encoded)
   {
 
     if (encoded == null)  return null;
 
     char[] base64Data = encoded.toCharArray();
     // remove white spaces
     int len = removeWhiteSpace(base64Data);
 
     if (len % FOURBYTE != 0) {
       return null;//should be divisible by four
     }
 
     int numberQuadruple = (len / FOURBYTE);
 
     if (numberQuadruple == 0)  return new byte[0];
 
     byte decodedData[] = null;
     byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
     char d1 = 0, d2 = 0, d3 = 0, d4 = 0;
 
     int i = 0;
     int encodedIndex = 0;
     int dataIndex = 0;
     decodedData = new byte[(numberQuadruple) * 3];
 
     for (; i < numberQuadruple - 1; i++)  {
 
       if (!isData((d1 = base64Data[dataIndex++]))
           || !isData((d2 = base64Data[dataIndex++]))
           || !isData((d3 = base64Data[dataIndex++]))
           || !isData((d4 = base64Data[dataIndex++])))
         return null;//if found "no data" just return null
 
       b1 = base64Alphabet[d1];
       b2 = base64Alphabet[d2];
       b3 = base64Alphabet[d3];
       b4 = base64Alphabet[d4];
 
       decodedData[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
       decodedData[encodedIndex++] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
       decodedData[encodedIndex++] = (byte) (b3 << 6 | b4);
     }
 
     if (!isData((d1 = base64Data[dataIndex++])) || !isData((d2 = base64Data[dataIndex++]))) {
       return null;//if found "no data" just return null
     }
 
     b1 = base64Alphabet[d1];
     b2 = base64Alphabet[d2];
 
     d3 = base64Data[dataIndex++];
     d4 = base64Data[dataIndex++];
     if (!isData((d3)) || !isData((d4))) {//Check if they are PAD characters
       if (isPad(d3) && isPad(d4))  { //Two PAD e.g. 3c[Pad][Pad]
         if ((b2 & 0xf) != 0){ //last 4 bits should be zero
           return null;
         }   
         byte[] tmp = new byte[i * 3 + 1];
         System.arraycopy(decodedData, 0, tmp, 0, i * 3);
         tmp[encodedIndex] = (byte) (b1 << 2 | b2 >> 4);
         return tmp;
       }  else if (!isPad(d3) && isPad(d4))  { //One PAD e.g. 3cQ[Pad]
         b3 = base64Alphabet[d3];
         if ((b3 & 0x3) != 0){//last 2 bits should be zero
           return null;
         }  
         byte[] tmp = new byte[i * 3 + 2];
         System.arraycopy(decodedData, 0, tmp, 0, i * 3);
         tmp[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
         tmp[encodedIndex] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
         return tmp;
       }  else {
         return null;//an error like "3c[Pad]r", "3cdX", "3cXd", "3cXX" where X
         // is non data
       }
     }  else { //No PAD e.g 3cQl
       b3 = base64Alphabet[d3];
       b4 = base64Alphabet[d4];
       decodedData[encodedIndex++] = (byte) (b1 << 2 | b2 >> 4);
       decodedData[encodedIndex++] = (byte) (((b2 & 0xf) << 4) | ((b3 >> 2) & 0xf));
       decodedData[encodedIndex++] = (byte) (b3 << 6 | b4);
      }
      return decodedData;
   }
   
/**
 * Converts the BASE64 encoded string found in the input stream into binary data in the output stream

 * 	Continues to read until end of string or a string of '\r\n\r\n' is found
 * @param in  - BASE64 string
 * @param out  - binary bytes
 * @throws IOException
 */
	  public static void decode(InputStream in, OutputStream out) throws IOException
	  {

		// Read input stream until end of file, "=" or 0x0d,0x0a 
		boolean quit = false;
		int bits=0;
		int nbits=0;
		int nbytes=0;
		int b;
		int[] mem = new int[4];

		while( !quit && (b=in.read()) != -1) {
		  byte c = b < 128 ? base64Alphabet[b] : -1;
		  mem[3] = mem[2];
		  mem[2] = mem[1];
		  mem[1] = mem[0];
		  mem[0] = b;
		  if (c != -1) {
			// Found base64 character
			nbytes++;
			bits=(bits<<6)|c;
			nbits+=6;
			if(nbits>=8) {
			  nbits-=8;
			  out.write(0xff&(bits>>nbits));
			}
		  } else if (b == 0x3d) {
			// Found '=' character
			quit = true;
			nbytes++;
			if (nbytes%4 != 0) {
			   if (in.read() == 0x3d) {
				 nbytes++;
			   } else { 
			     throw new IOException("Stream not terminated with correct number of '='");
			   }
			}
		  } else if (mem[0] == 0x0a && mem[1] == 0x0d && mem[2] == 0x0a && mem[3] == 0x0d ) {
			// Found '\r\n\r\n'
				quit = true;
		  }
		}
    
		if (nbytes%4 != 0)
		  throw new IOException("Base64 stream not a multiple of 4 characters");
	  }

/**
 * 	Encodes the binary data contained in the input stream into a BASE64 encoded stream
 * 	Produces lines of length equal to 76 characters
 * @param in  - binary data
 * @param out - Base 64 string
 * @throws IOException
 */

	public static void encode(InputStream in, OutputStream out )  throws IOException {
			encode(in,out,76);
	  }
/**
 * 	Encodes the binary data contained in the input stream into a BASE64 encoded stream
 *  Creates line breaks according to the len parameter
 * @param in - binary data
 * @param out - Base 64 string
 * @param len - length of each line after encoding. Must be a multiple of 4.
 * @throws IOException
 */	  

	  public static void encode(InputStream in, OutputStream out, int len) 
		throws IOException {

		// Check that length is a multiple of 4 bytes
		if(len%4!=0)  throw new IllegalArgumentException("Length must be a multiple of 4");

		// Read input stream until end of file
		int bits=0;
		int nbits=0;
		int nbytes=0;
		int b;

		while( (b=in.read()) != -1) {
		  bits=(bits<<8)|b;
		  nbits+=8;
		  while(nbits>=6) {
			nbits-=6;
			out.write(lookUpBase64Alphabet[0x3f&(bits>>nbits)]);
			nbytes ++;
			// New line
			if (len !=0 && nbytes>=len) {
			  out.write(0x0d);
			  out.write(0x0a);
			  nbytes -= len;
			}
		  }
		}
	// add padding when applicable
		switch(nbits) {
		case 2:
		  out.write(lookUpBase64Alphabet[0x3f&(bits<<4)]);
		  out.write(0x3d); // 0x3d = '='
		  out.write(0x3d);
		  break;
		case 4:
		  out.write(lookUpBase64Alphabet[0x3f&(bits<<2)]);
		  out.write(0x3d);
		  break;
		}
	
		if (len != 0) {
			  if (nbytes != 0) {
				out.write(0x0d);
				out.write(0x0a);
			  }
			  out.write(0x0d);
			  out.write(0x0a);
			}
	  }
 
   /**
    * remove WhiteSpace from MIME containing encoded Base64 data.
    * 
    * @param data the byte array of base64 data (with WS)
    * @return the new length
    */
   protected static int removeWhiteSpace(char[] data)
   {
     if (data == null)
       return 0;
 
     // count characters that's not whitespace
     int newSize = 0;
     int len = data.length;
     for (int i = 0; i < len; i++)
     {
       if (!isWhiteSpace(data[i]))
         data[newSize++] = data[i];
     }
     return newSize;
   }
   
 
 /**
  * Tests the character to see if it is valid under BASE64 usage
  * @param octect
  * @return
  */
 
   protected static boolean isBase64(char octect)
   {
	 return (isData(octect)||isWhiteSpace(octect) || isPad(octect) );
   }

/**
 * Checks for space or end of line characters
 * @param octect
 * @return
 */
   protected static boolean isWhiteSpace(char octect)
   {
	 return (octect == 0x20 || octect == 0xd || octect == 0xa || octect == 0x9);
   }
 /**
  * Checks to see if character is an '=' for Base64 padding
  * @param octect
  * @return
  */
   protected static boolean isPad(char octect)
   {
	 return (octect == PAD);
   }
 /**
  * Check to see if the character is found in mapping
  * @param octect
  * @return
  */
 
   protected static boolean isData(char octect)
   {
	 return (base64Alphabet[octect] != -1);
   }
   
   static private final int     BASELENGTH           = 255;
   static private final int     LOOKUPLENGTH         = 64;
   static private final int     TWENTYFOURBITGROUP   = 24;
   static private final int     EIGHTBIT             = 8;
   static private final int     SIXTEENBIT           = 16;
   static private final int     FOURBYTE             = 4;
   static private final int     SIGN                 = -128;
   static private final char    PAD                  = '=';
   static final private byte[]  base64Alphabet       = new byte[BASELENGTH];
   static final private char[]  lookUpBase64Alphabet = new char[LOOKUPLENGTH];
 
   static
   {
 
	 for (int i = 0; i < BASELENGTH; i++)
	 {
	   base64Alphabet[i] = -1;
	 }
	 for (int i = 'Z'; i >= 'A'; i--)
	 {
	   base64Alphabet[i] = (byte) (i - 'A');
	 }
	 for (int i = 'z'; i >= 'a'; i--)
	 {
	   base64Alphabet[i] = (byte) (i - 'a' + 26);
	 }
 
	 for (int i = '9'; i >= '0'; i--)
	 {
	   base64Alphabet[i] = (byte) (i - '0' + 52);
	 }
 
	 base64Alphabet['+'] = 62;
	 base64Alphabet['/'] = 63;
 
	 for (int i = 0; i <= 25; i++)
	   lookUpBase64Alphabet[i] = (char) ('A' + i);
 
	 for (int i = 26, j = 0; i <= 51; i++, j++)
	   lookUpBase64Alphabet[i] = (char) ('a' + j);
 
	 for (int i = 52, j = 0; i <= 61; i++, j++)
	   lookUpBase64Alphabet[i] = (char) ('0' + j);
	 lookUpBase64Alphabet[62] = '+';
	 lookUpBase64Alphabet[63] = '/';
   }

   
   /*
    * Byte Array implementation
    *  @author Jeffrey Rodriguez
    * @author Sandy Gao
    * @version $Id: Base64.java,v 1.1 2005/06/23 18:22:48 pflynn Exp $
    * 
    * 
    * 
    * 
    * Streaming implementation baed on KNOPFLERFISH project code
    * 
    * 
    * 
	* Copyright (c) 2003, KNOPFLERFISH project
	* All rights reserved.
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following
	* conditions are met:
	*
	* - Redistributions of source code must retain the above copyright
	*   notice, this list of conditions and the following disclaimer.
	*
	* - Redistributions in binary form must reproduce the above
	*   copyright notice, this list of conditions and the following
	*   disclaimer in the documentation and/or other materials
	*   provided with the distribution.
	*
	* - Neither the name of the KNOPFLERFISH project nor the names of its
	*   contributors may be used to endorse or promote products derived
	*   from this software without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
	* OF THE POSSIBILITY OF SUCH DAMAGE.
	*/
   
 }


