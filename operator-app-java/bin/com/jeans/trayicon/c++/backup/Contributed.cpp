HICON CreateIconFromIntArray(JNIEnv *env, jintArray array, jint width, jint height) {
        HICON hIcon; // icon handle 

        //To protect against java sending a dud image
        if ((width == -1) || (height == -1))
                return NULL;

        //Get the java array as a contiguous 32 bit array
        jsize len = env->GetArrayLength(array);
        jint *im  = env->GetIntArrayElements(array, 0);
        unsigned long *data = (unsigned long *)im;

        //Set up the header for creating our 24 bit colour bitmap
        BITMAPINFOHEADER bih;
        bih.biSize          = sizeof(BITMAPINFOHEADER);
        bih.biWidth         = width;
        bih.biHeight        = height;
        bih.biPlanes        = 1;
        bih.biBitCount      = 24;
        bih.biCompression   = BI_RGB;
        bih.biSizeImage     = 0;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed       = 0;
        bih.biClrImportant  = 0;

        //Create memory DC
        HDC hdc = CreateCompatibleDC(NULL);

        //Make the 24-bit DIB
        HBITMAP hBitmapXOR = CreateDIBSection(hdc, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (LPVOID *)NULL, NULL, 0);
        //Select it into the DC so we can draw onto it
        SelectObject(hdc, hBitmapXOR);

        //calloc memory to be used to create a monochrome bitmask
        unsigned char *andMask = (unsigned char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (width*height/8)+1);

        //Loop through the given pixels and draw onto the colour and mono bitmaps
        unsigned int pixel; //32-bits
        unsigned char red, green, blue, alpha; //8-bits
        for (int row=0; row < height; row++)
        {
                for (int col = 0; col < width; col++)
                {
                        pixel = data[(row*width)+col];
                        alpha = (unsigned char)((pixel >> 24) & 0x000000ff);
                        red   = (unsigned char)((pixel >> 16) & 0x000000ff);
                        green = (unsigned char)((pixel >>  8) & 0x000000ff);
                        blue  = (unsigned char)( pixel        & 0x000000ff);

                        if (alpha == 0xFF) //Pixel is not transparent
                        {
                                SetPixel(hdc, col, row, RGB(red, green, blue));
                        }
                        else //pixel is transparent
                        {
                                int p = (row*width) + col;
                                if (andMask)
                                        andMask[p/8] |= 1 << (7-(p%8));
                        }
                }
        }
	//Create the monochrome bitmask
        HBITMAP hBitmapAND = CreateBitmap(width, height, 1, 1, andMask);

        //Create an icon
        ICONINFO ii;
        ii.fIcon    = TRUE;
        ii.xHotspot = 0;
        ii.yHotspot = 0;
        ii.hbmMask  = hBitmapAND;
        ii.hbmColor = hBitmapXOR;
        hIcon = CreateIconIndirect(&ii);

        //Release all of our memory / objects
        env->ReleaseIntArrayElements(array, im, 0); //Release java array
        ReleaseDC(NULL, hdc); //Release the memory DC
        HeapFree(GetProcessHeap(), 0, andMask); //Free the memory to create the bit mask
        DeleteObject(hBitmapAND); //Destroy our bitmaps
        DeleteObject(hBitmapXOR);

        //Return the handle to the new icon
        return hIcon;
}


