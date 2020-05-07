#include "gui.h"

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
		SDL_Surface *new = resize(img);
		SDL_SaveBMP(new,"image_resize");
		gtk_image_set_from_file (GTK_IMAGE (image), "image_resize");
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

char * UpdatePath(char *filepath,size_t len,char c)
{
    char *newpath = malloc(len*sizeof(char));
    for (size_t i = 0; i < len; i++) {
        if (i != 13) {
            newpath[i] = filepath[i];
        }
        else{
            newpath[i] = c;
        }
    }
    return newpath;
}

int TrainNeuralNetwork(){
    struct network *network = InitializeNetwork(30*30,20,52);
    init_sdl();
    char *filepath = "img/training/A0.png";
    char expected_result[52] = {'A','a','B','b','C','c','D','d','E','e','F','f','G',\
    'g','H','h','I','i','J','j','K','k','L','I','M','m','N','n','O','o','P','p',\
    'Q','q','R','r','S','s','I','t','U','u','V','v','W','w','X','x','Y','y','Z','z'};
    int trainingSetOrder[52];
    for (size_t i = 0; i < 52; i++)
    {
        trainingSetOrder[i] = i;
    }
    int nb = 1000;
    int step = 0;
    for (size_t i = 0; i < (size_t)nb; i++)
    {
        shuffle(trainingSetOrder,52);
        progressBar(step,nb);
        step++;
        for (size_t index = 0; index < 52; index++)
        {
            size_t input_index = trainingSetOrder[index];
            filepath = UpdatePath(filepath,(size_t)strlen(filepath),expected_result[input_index]);

            SDL_Surface* image = load__image(filepath);

            image = black_and_white(image);

            DrawRedLines(image);

            int BlocCount = CountBlocs(image);
            SDL_Surface ***chars = malloc(sizeof(SDL_Surface**) * BlocCount);
            SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocCount);
            int *charslen = DivideIntoBlocs(image,blocs,chars, BlocCount);

            for (int j = 0; j < BlocCount; ++j) {
                SDL_FreeSurface(blocs[j]);
            }

            int **chars_matrix =  NULL;
            int chars_count = ImageToMatrix(chars,&chars_matrix, charslen, BlocCount);

            ExpectedOutput(network,expected_result[input_index]);
            InputImage(network,0,&chars_matrix);
            forward_pass(network);
            //PrintState(network,expected_result[input_index],RetrieveChar(IndexAnswer(network)));
            back_propagation(network);
            updateweightsetbiases(network);
            printf("%d",chars_count);
      }
    }
    printf("\n");
    printf("\e[?25h");
    save_network("source/Xor/ocrwb.txt",network);
    free(network);
    return EXIT_SUCCESS;
}

int OCR(GtkTextBuffer *buffer){
    struct network *network = InitializeNetwork(30*30,20,52);
    init_sdl();
    if(cfileexists(filename)){
        SDL_Surface* image = load__image(filename);
        //SDL_Surface* screen_surface = display_image(image);
        //wait_for_keypressed();
        image = black_and_white(image);
        //screen_surface = display_image(image);
        //wait_for_keypressed();
        //SDL_SaveBMP(image,"binarisation.bmp");
        DrawRedLines(image);
        int BlocCount = CountBlocs(image);
        SDL_Surface ***chars = malloc(sizeof(SDL_Surface**) * BlocCount);
        SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocCount);
        int *charslen = DivideIntoBlocs(image,blocs,chars, BlocCount);
        SDL_SaveBMP(image,"segmentation.bmp");
        if(image->w > 560 && image->h > 560){
            SDL_Surface *new = resize(image);
            SDL_SaveBMP(new,"segmentation_resize");
            gtk_image_set_from_file (GTK_IMAGE (image), "segmentation_resize");
	    }
        else{
            gtk_image_set_from_file (GTK_IMAGE (image), "segmentation.bmp");
        }

        for (int j = 0; j < BlocCount; ++j) {
            SDL_FreeSurface(blocs[j]);
        }
        //SDL_Surface* new_image=load_image("segmentation.bmp");
        //screen_surface = display_image(new_image);
        //wait_for_keypressed();
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
        //SDL_FreeSurface(new_image);
        //SDL_FreeSurface(screen_surface);
        SDL_Quit();
        gtk_text_buffer_set_text (buffer,result,strlen(result));
        free(network);
        return EXIT_SUCCESS;
    }
    return 1;
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

    gtk_window_set_title(GTK_WINDOW(main_window), "Welcome to OCR-GANG");
    gtk_widget_show_all(main_window);
    gtk_main();
}
