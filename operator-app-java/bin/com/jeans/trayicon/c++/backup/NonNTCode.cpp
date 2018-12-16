/**
 * Backup code (doesn't work in NT - why?)
 *
 * void setPixel(unsigned char *bitmap, long offs) {
 *		int ofs = offs / 8;   
 *		int idx = offs % 8;	
 *		bitmap[ofs] |= 1 << (7-idx);	
 * }
 * 
 * class IconData {
 * private:
 * 	unsigned char *bitmap;
 * 	unsigned char *and_bits;
 * 	int wd, hi;
 * public:
 * 	IconData();
 * 	~IconData();
 * 	int setData(unsigned long *data, int wd, int hi);
 * 	HICON makeIcon(HINSTANCE hInst);
 * };
 *  
 * IconData::IconData() {
 * 	wd = hi = 0;
 * 	and_bits = bitmap = NULL;
 * }
 *
 * IconData::~IconData() {
 * 	   if (bitmap != NULL) delete bitmap;
 * 	   if (and_bits != NULL) delete and_bits;
 * }
 *
 * HICON IconData::makeIcon(HINSTANCE hInst) {
 * 	   return CreateIcon(hInst, wd, hi, 1, 24, and_bits, bitmap);
 * }
 *
 * int IconData::setData(unsigned long *data, int wd, int hi) {
 * 	   this->wd = wd;
 * 	   this->hi = hi;
 * 	   int and_size = wd*hi/8+1;
 * 	   int bitmap_size = wd*hi*24/8+1;
 * 	   int len = wd*hi;
 * 	   and_bits = new unsigned char[and_size];
 * 	   bitmap = new unsigned char[bitmap_size];
 * 	   if (and_bits != NULL && bitmap != NULL) {
 * 		   for (int and = 0; and < and_size; and++) and_bits[and] = 0;
 * 		   for (int i=0; i<len; i++) {
 * 			   unsigned long pixel = data[i];
 * 			   unsigned char alpha = (unsigned char)((pixel >> 24) & 0xff);
 * 			   unsigned char red   = (unsigned char)((pixel >> 16) & 0xff);
 * 			   unsigned char green = (unsigned char)((pixel >>  8) & 0xff);
 * 			   unsigned char blue  = (unsigned char)(pixel& 0xff);
 * 			   if (alpha != 0xFF) {
 * 				   bitmap[i*3] = 0;
 * 				   bitmap[i*3+1] = 0;
 * 				   bitmap[i*3+2] = 0;
 * 				   setPixel(and_bits, i);
 * 			   } else {
 * 				   bitmap[i*3] = blue;
 * 				   bitmap[i*3+1] = green;
 * 				   bitmap[i*3+2] = red;
 * 			   }
 * 		   }
 * 		   return 0;
 * 	   } else {
 * 		   if (and_bits != NULL) delete and_bits;
 * 		   if (bitmap != NULL) delete bitmap;
 * 		   and_bits = NULL; bitmap = NULL;
 * 		   return -1;
 * 	   }
 * }
 *
 */ 
