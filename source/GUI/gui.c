#include "gui.h"
#include "../network/tools.h"


gchar *filename = "";
char *text = "";
GtkWidget *parent;

void save_text(GtkButton *button, GtkTextBuffer *buffer)
{
    UNUSED(button);
    UNUSED(buffer);
    GtkWidget *dialog;
    GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
    dialog = gtk_file_chooser_dialog_new ("Save Text ",
                        GTK_WINDOW (toplevel),
                        GTK_FILE_CHOOSER_ACTION_SAVE,
                        "Cancel", GTK_RESPONSE_CANCEL,
                        "Save", GTK_RESPONSE_ACCEPT,
                        NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        /* set the contents of the file to the text from the buffer */
        if (filename != NULL)
            g_file_set_contents (filename, text, strlen(text), NULL);
        else
            g_file_set_contents (filename, text, strlen(text), NULL);
  }
  gtk_widget_destroy (dialog);

}

void load_image(GtkButton *button, GtkImage *image)
{
    if(strcmp(filename,"") == 0)
        return;
    UNUSED(button);
    SDL_Surface *img = load__image((char *)filename);
    if(img->w > 560 && img->h > 560){
        float wi = img->w;
        float hi = img->h;
        float max_h = 560.;
        float max_w = 560.;
        float best;
        if (max_w / wi < max_h / hi)
            best = max_w / wi;
        else
            best = max_h / hi;
        int new_w = wi * best;
        int new_h = hi * best;
        //printf("%d %d",new_w,new_h);
        SDL_Surface *new = resize(img,new_w,new_h);
        SDL_SaveBMP(new,"image_resize.bmp");
        gtk_image_set_from_file (GTK_IMAGE (image), "image_resize.bmp");
    }
    else{
        gtk_image_set_from_file (GTK_IMAGE (image), filename);
    }
}
//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

void open_image(GtkButton *button, GtkLabel *text_label)
{
  GtkWidget *label = (GtkWidget *) text_label;
	GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET(button));
	//GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                GTK_WINDOW (toplevel),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                "Open", GTK_RESPONSE_ACCEPT,
	                                                "Cancel", GTK_RESPONSE_CANCEL,
	                                                NULL);

	//gtk_file_filter_add_pixbuf_formats (filter);
	//gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            gtk_label_set_text(GTK_LABEL(label),filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy(dialog);
}


int TrainNeuralNetwork(){
    PrepareTraining();
    struct network *network = InitializeNetwork(28*28,20,52,"source/OCR/ocrwb.txt");
    char *filepath = "img/training/maj/A0.txt\0";
    char expected_result[52] = {'A','a','B','b','C','c','D','d','E','e','F','f','G',\
    'g','H','h','I','i','J','j','K','k','L','I','M','m','N','n','O','o','P','p',\
    'Q','q','R','r','S','s','I','t','U','u','V','v','W','w','X','x','Y','y','Z','z'};
    int nb = 5000;
    int step = 0;
    for (size_t number = 0; number < (size_t)nb; number++)
    {
        for (size_t i = 0; i < 52; i++)
        {
            for (size_t index = 0; index < 4; index++)
            {
                step++;
                progressBar(step,nb*52*4);
                filepath = updatepath(filepath,(size_t)strlen(filepath),expected_result[i],index);
                ExpectedOutput(network,expected_result[i]);
                InputFromTXT(filepath,network);
                forward_pass(network);
                //PrintState(expected_result[i],RetrieveChar(IndexAnswer(network)));
                back_propagation(network);
                updateweightsetbiases(network);
            }
        }
  }
  printf("\n");
  printf("\e[?25h");
  save_network("source/OCR/ocrwb.txt",network);
  free(network);
  return EXIT_SUCCESS;
}

int OCR(GtkButton *button,GtkTextBuffer *buffer){
    UNUSED(button);
    struct network *network = InitializeNetwork(28*28,20,52,"source/OCR/ocrwb.txt");
    init_sdl();
    SDL_Surface* image = load__image((char*)filename);
    image = black_and_white(image);
    g_print("Black and White and Binarization Done !\n");
    DrawRedLines(image);
    int BlocCount = CountBlocs(image);
    SDL_Surface ***chars = malloc(sizeof(SDL_Surface**) * BlocCount);
    SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocCount);
    int *charslen = DivideIntoBlocs(image,blocs,chars, BlocCount);
    SDL_SaveBMP(image,"segmentation.bmp");
    g_print("Segmentation Done !\n");
    for (int j = 0; j < BlocCount; ++j) {
        SDL_FreeSurface(blocs[j]);
    }

    int **chars_matrix =  NULL;
    int chars_count = ImageToMatrix(chars,&chars_matrix, charslen, BlocCount);
    char *result = calloc(chars_count,sizeof(char));

    for (size_t index = 0; index < (size_t)chars_count; index++) {
        int is_espace = InputImage(network,index,&chars_matrix);
        if (!is_espace) {
          forward_pass(network);
          size_t index_answer = IndexAnswer(network);
          result[index] = RetrieveChar(index_answer);
        }
        else{
          result[index] = ' ';
        }
    }
    SDL_Quit();
    g_print("OCR Done !\n");
    text = result;
    gtk_text_buffer_set_text (buffer,result,strlen(result));
    free(network);
    return EXIT_SUCCESS;
}

void InitGUI(int argc, char *argv[])
{
	//Init variables
    GtkWidget *main_window;
	SGlobalData data;
	//Init GTK
	gtk_init(&argc, &argv);
	//Build from .glade
    data.builder = gtk_builder_new();
    gtk_builder_add_from_file(data.builder, "gui.glade", NULL);
	//Get main_window
	main_window =  GTK_WIDGET(gtk_builder_get_object(data.builder,"main_window"));
    parent = main_window;
	//Connect signals
	gtk_builder_connect_signals(data.builder, &data);
    g_signal_connect(main_window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_window_set_title(GTK_WINDOW(main_window), "Welcome to OCR-GANG");
    gtk_widget_show_all(main_window);
    gtk_main();
}
