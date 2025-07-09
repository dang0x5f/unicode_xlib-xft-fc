#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <fontconfig/fontconfig.h>

void test01(void); // Random function calls
int test02(void);  // FontConfig Pattern Match Example

int main(void)
{
    /* test01(); */
    printf("return: %d\n",test02());

    return(0);
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
