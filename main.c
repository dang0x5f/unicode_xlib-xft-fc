#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>

void test01(void);  // Random function calls
int test02(void);   // FontConfig Pattern Match Example
void test03(void);  // characters 0-2048

int main(void)
{
    /* test01(); */
    /* printf("return: %d\n",test02()); */
    test03();

    return(0);
}

void test03(void)
{
    XftFont *font;
    Window window;

    Display *display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    Colormap colormap = DefaultColormap(display,screen_num);

    int valuemask = CWEventMask|CWBackPixel;
    XSetWindowAttributes attributes = { 
        .background_pixel = 0xfffdd0,
        .event_mask = ExposureMask|SubstructureNotifyMask
    };
    
    font = XftFontOpenName(display,screen_num,"Deja Vu Sans Mono:pixelsize=12");
    window = XCreateWindow(display,DefaultRootWindow(display), 0,0,  400,300,  
                                         5, DefaultDepth(display,screen_num), 
                                         InputOutput, 
                                         DefaultVisual(display, screen_num),
                                         valuemask, &attributes);
    XftDraw *draw = XftDrawCreate(display, window, 
                        DefaultVisual(display,screen_num),colormap);

    XftColor color;
    XftColorAllocName(display,DefaultVisual(display,screen_num),colormap,"#555555",&color);

    XMapWindow(display,window);
    XSync(display,false);

    XEvent ev;
    while(1){
        XNextEvent(display,&ev);

        switch(ev.type){
            case Expose:
                /* for(int line=1; line<10; ++line) */
                /* XftDrawStringUtf8(draw,&color,font,0, */
                /*         font->ascent*line+font->descent*(line-1),"Line",4); */

                Window ret_win;
                int ret_x, ret_y, ret_w, ret_h, ret_border, ret_depth;
                XGetGeometry(display,window,&ret_win,&ret_x, &ret_y, 
                             &ret_w, &ret_h, &ret_border, &ret_depth);

                XftGlyphFontSpec spex;

                int line = 1, xpos = 0, ypos = font->height*line;
                uint32_t unicode_c;
                for(int d=0; d<16384; ++d){
                    unicode_c = (uint32_t)0x00 << 24 |
                                (uint32_t)0x00 << 16 |
                                (uint32_t)0x00 <<  8 |
                                (uint32_t)   d <<  0 ;
                    
                    uint32_t idx = XftCharIndex(display, font, unicode_c);
                    if(idx){
                        spex.font = font;
                        spex.glyph = idx;
                        if(xpos + font->max_advance_width > ret_w){
                            ++line;
                            xpos = 0;
                            ypos = font->height*line;
                        }
                        spex.x = xpos;
                        spex.y = ypos;
                        XftDrawGlyphFontSpec(draw,&color,&spex,1);
                        xpos += font->max_advance_width;
                    }
                }

                for(int d=0; d<100; ++d){
                    unicode_c = (uint32_t)0x00 << 24 |
                                (uint32_t)0x00 << 16 |
                                (uint32_t)0x25 <<  8 |
                                (uint32_t)0x88 <<  0 ;
                    
                    uint32_t idx = XftCharIndex(display, font, unicode_c);
                    if(idx){
                        spex.font = font;
                        spex.glyph = idx;
                        if(xpos + font->max_advance_width > ret_w){
                            ++line;
                            xpos = 0;
                            ypos = font->height*line;
                        }
                        spex.x = xpos;
                        spex.y = ypos;
                        XftDrawGlyphFontSpec(draw,&color,&spex,1);
                        xpos += font->max_advance_width;
                    }
                }
                
                /* uint32_t idx = XftCharIndex(display, font, unicode_c); */
                /* if(idx){ */
                /*     spex.font = font; */
                /*     spex.glyph = idx; */
                /*     spex.x = xpos; */
                /*     spex.y = ypos; */
                /*     xpos += font->max_advance_width; */
                /*     X */
                /* } */

                break;
        }

    }
    
}

/* FontConfig Pattern Match Example */
int test02(void)
{
    // load in font config
    FcConfig *config = FcInitLoadConfigAndFonts();
    if(!config){
        fprintf(stderr,"(!) Failed to load config.\n");
        return(EXIT_FAILURE);
    }

    // create pattern to search config
    FcPattern *pattern = FcPatternCreate();
    if(!pattern){
        fprintf(stderr,"(!) Failed to create pattern.\n");
        FcConfigDestroy(config);
        return(EXIT_FAILURE);
    }

    // add value to pattern to help search for a font
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)"IBM Plex Mono");

    // search for font in config using above pattern, return match
    FcResult result;
    FcPattern *match = FcFontMatch(config, pattern, &result);
    if(!match || result != FcResultMatch){
        FcPatternDestroy(pattern);
        FcConfigDestroy(config);
        return(EXIT_FAILURE);
    }

    FcChar8 *filename = NULL;
    if(FcPatternGetString(match, FC_FILE, 0, &filename) == FcResultMatch){
        printf("[Font File]: %s\n", filename);
    }

    FcChar8 *family = NULL;
    if(FcPatternGetString(match, FC_FAMILY, 0, &family) == FcResultMatch){
        printf("[Font Family]: %s\n", family);
    }

    // cleanup
    FcPatternDestroy(match);
    FcPatternDestroy(pattern);
    FcConfigDestroy(config);
    FcFini();

    return(0);
}


/* Testing random functions */
void test01(void)
{
    /* FcConfig *fcc = FcInitLoadConfig(); */
    /* FcConfig *fcc = FcInitLoadConfigAndFonts(); */
    
    if(FcInit())
        printf("FcInit() succeeded\n");
    else
        printf("FcInit() failed\n");

    printf("Version: %d\n", FcGetVersion());

    if(FcInitReinitialize())
        printf("FcInitReinitialize() succeeded\n");
    else
        printf("FcInitReinitialize() failed\n");

    FcFini();
}
