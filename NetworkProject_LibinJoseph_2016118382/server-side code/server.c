#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>


#define BUF_SIZE 1024
void error_handling(char *message);
int serv_sock;
  int clnt_sock;

  char msg[BUF_SIZE];
  int  str_len, i;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;

  socklen_t clnt_addr_size;
	int counter=0;
	GtkWidget  *doorclosed,*label2,*doorCl,*la,*activD,*r,*activM,*motDetect,*motOk;
static gboolean on_timeout (gpointer user_data)
{

	if(counter == 0){
		  clnt_addr_size = sizeof(clnt_addr);
			clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
			if (clnt_sock == -1) {
		 error_handling("accept() error");
	 } else {
		 printf("Connected client %d \n", i + 1);
		 counter =1;
	 }
 }
 if(counter == 1){
	 GtkLabel *label = GTK_LABEL (user_data);
	 GtkLabel *lab2 = GTK_LABEL (label2);
	 GtkLabel *motlab = GTK_LABEL (motOk);


	 GtkImage *img = GTK_IMAGE(doorclosed);
	 GtkImage *img2 = GTK_IMAGE(motDetect);

str_len = read(clnt_sock, &msg, BUF_SIZE);
		gchar *text = g_strdup_printf (msg);
		char *array[2];
		if(strlen(text)<=12){
	     int i = 0;
	     char *p = strtok (text, ",");


	     while (p != NULL)
	     {
	         array[i++] = p;
	         p = strtok (NULL, ",");
	     }
		 }

		double distance = atof(array[0]);
		int sonar=0,mts=atoi(array[1]);
		if(distance >=5){
				sonar=1;
				gtk_image_set_from_file(img,"doorOpen.png");
				distance=roundf(distance * 100) / 100;
				gchar *text2 = g_strdup_printf ("your door is opened by\n %.2f cms !\n",distance-5);
				distance=0;
				gtk_label_set_label (label, text2);
			}
		else if(distance < 5 && distance >0){
			gtk_label_set_label (label, "your door is closed");
			gtk_image_set_from_file(img,"doorClosed.png");
			sonar=0;
		}
		if(mts==1){
				gtk_image_set_from_file(img2,"motDetected.png");
				gtk_label_set_label (motlab, "Motion Detected!");
		}else if(mts==0){
			gtk_image_set_from_file(img2,"motionNormal.png");
			gtk_label_set_label (motlab, "No Motion Detected");
		}
		if(sonar==1 || mts==1){
			gtk_label_set_label (lab2, "Your Home is at risk ⚠️");
		}
		else
		gtk_label_set_label (lab2, "Your Home is secure");

		while (gtk_events_pending())
			gtk_main_iteration();
		g_free (text);
}

		return G_SOURCE_CONTINUE;

}
int clicked=0;
static void button_clicked_door(GtkWidget* widget, gpointer data)
{
		if(clicked == 0){
				gtk_widget_hide(doorclosed);
				gtk_widget_hide(doorCl);
				gtk_button_set_label(GTK_BUTTON(la), "Activate Door sensor");
				gtk_label_set_label(GTK_LABEL(activD),"Door sensor inactive");
				clicked=1;
			}
		else{
			gtk_widget_show(doorclosed);
			gtk_widget_show(doorCl);
			gtk_button_set_label(GTK_BUTTON(la), "Deactivate Door sensor");
			gtk_label_set_label(GTK_LABEL(activD),"Door sensor active");

			clicked=0;
		}


}

int clicked2=0;
static void button_clicked_motion(GtkWidget* widget, gpointer data)
{
		if(clicked2 == 0){
				gtk_widget_hide(motOk);
				gtk_widget_hide(motDetect);
				gtk_button_set_label(GTK_BUTTON(r), "Activate Motion sensor");
				gtk_label_set_label(GTK_LABEL(activM),"Motion sensor inactive");
				clicked2=1;
			}
		else{
			gtk_widget_show(motOk);
			gtk_widget_show(motDetect);
			gtk_button_set_label(GTK_BUTTON(r), "Deactivate Motion sensor");
			gtk_label_set_label(GTK_LABEL(activM),"Motion sensor active");

			clicked2=0;
		}


}

int main(int argc, char *argv[])
{




	  if (argc != 2) {
	    printf("Usage : %s <port>\n", argv[0]);
	    exit(1);
	  }

	  serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	  if (serv_sock == -1) {
	    error_handling("socket() error");
	  }

	  memset(&serv_addr, 0, sizeof(serv_addr));
	  serv_addr.sin_family      = AF_INET;
	  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	  serv_addr.sin_port        = htons(atoi(argv[1]));

	  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
	    error_handling("bind() error");
	  }

	  if (listen(serv_sock, 5) == -1)  {
	    error_handling("listen() error");
	  }


	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "HomeSecure");
	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *k;
	k= gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), k);


	GtkWidget *label,*dooropen,*mthuman,*doorsens,*line;
	la = gtk_button_new_with_label ("Deactivate door Sensor");
	gtk_fixed_put (GTK_FIXED (k), la,50,200);
	gtk_widget_set_size_request(la, 120, 60);
	g_signal_connect(la,"clicked", G_CALLBACK(button_clicked_door), NULL);


	label = gtk_label_new("🏠 Realtime Home Security System");
	gtk_fixed_put (GTK_FIXED (k), label,120,15);


	label2 = gtk_label_new("Your home is secure");
	gtk_fixed_put (GTK_FIXED (k), label2,250,75);



	r = gtk_button_new_with_label ("Deactivate Motion Sensor");
	gtk_fixed_put (GTK_FIXED (k), r,440,200);
	gtk_widget_set_size_request(r, 120, 60);
	g_signal_connect(r,"clicked", G_CALLBACK(button_clicked_motion), NULL);


	GtkCssProvider *provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_path (provider,"gtk-widgets.css", NULL);


	doorclosed = gtk_image_new_from_file("doorClosed.png");
	gtk_fixed_put (GTK_FIXED (k), doorclosed,50,350);
	gtk_widget_set_size_request(doorclosed, 40, 40);

	motDetect = gtk_image_new_from_file("motionNormal.png");
	gtk_fixed_put (GTK_FIXED (k), motDetect,440,350);
	gtk_widget_set_size_request(motDetect, 40, 40);

	motOk = gtk_label_new("");
	gtk_fixed_put (GTK_FIXED (k), motOk,540,380);

	doorCl = gtk_label_new("null");
	gtk_fixed_put (GTK_FIXED (k), doorCl,150,370);
	gtk_label_set_text((GtkLabel*)doorCl,msg);
	g_timeout_add (210 ,on_timeout,doorCl);



	activD = gtk_label_new("Door Sensor Active");
	gtk_fixed_put (GTK_FIXED (k), activD,60,270);

	activM = gtk_label_new("Motion Sensor Active");
	gtk_fixed_put (GTK_FIXED (k), activM,440,270);


	line = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_fixed_put (GTK_FIXED (k), line,370,200);
	gtk_widget_set_size_request(line, 40, 300);

	gtk_widget_set_size_request(GTK_WIDGET(window),800,500);

	GtkStyleContext *context,*Heading,*body,*lb2;
	context = gtk_widget_get_style_context(la);
	lb2 = gtk_widget_get_style_context(label2);
	Heading = gtk_widget_get_style_context(label);
	body=gtk_widget_get_style_context(window);
	gtk_style_context_add_class(context,"la");
	gtk_style_context_add_class(lb2,"lab2");
	gtk_style_context_add_class(Heading,"lab");
	gtk_style_context_add_class(body,"bodyel");
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);



	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_main();




	return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
