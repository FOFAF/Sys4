#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 3490 /* 서버 포트 */
#define MAXDATASIZE 100

typedef struct
{
	GtkWidget * entry, * textview;
} Widgets;

Widgets *w;
int csock, numbytes;

void *t_function(void *data)
{
	char buf[MAXDATASIZE];
	while(1){
	numbytes = -1;
	memset(buf, 0, MAXDATASIZE);
	if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) == -1) {
		continue;
	}
	char* res="(RESIVED)";
	strcat(buf, res);
	GtkTextBuffer *buffer; 
	GtkTextMark *mark;
	GtkTextIter iter;
	const gchar *text;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(w->textview));
	text = buf;//gtk_entry_get_text (GTK_ENTRY (w->entry));
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);

	/* 버퍼에 텍스트가 있으면 뉴라인 삽입 */
	if (gtk_text_buffer_get_char_count(buffer))
		gtk_text_buffer_insert (buffer, &iter, "\n", 1);
	gtk_text_buffer_insert (buffer, &iter, text, -1);
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW (w->textview) , mark);
}
}

void CLIENT(){

	char buf[MAXDATASIZE];
	struct sockaddr_in serv_addr;
	int len;

	if ((csock = socket(AF_INET, SOCK_STREAM, 0))== -1) {
		perror("socket");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORT);
	/* 서버 연결 */
	if (connect(csock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
		perror ("connect");
		exit (1);
	}
	

}

static void insert_text (GtkButton *button, Widgets *w)
{
	const gchar *text;
	text = gtk_entry_get_text (GTK_ENTRY (w->entry));
	int len;
	len = strlen(text);
	if ( send(csock, text, len, 0) != len) {
		return;
	}

}

int main (int argc, char *argv[])
{
 	CLIENT();
	int thr_id;
 	pthread_t p_thread;
   	int a = 1;
	thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    	if (thr_id < 0)
    	{
       		perror("thread create error : ");
        		exit(0);
    	}

	GtkWidget *window, *scrolled_win, *hbox, *vbox, *insert; 
	w= g_slice_new (Widgets);
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window), "텍스트뷰" );
	gtk_container_set_border_width (GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, -1, 200);

	w->textview = gtk_text_view_new ();
	w->entry = gtk_entry_new ();

	insert = gtk_button_new_with_label ("Insert Text");
	g_signal_connect (insert, "clicked", G_CALLBACK (insert_text), (gpointer) w);
	scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolled_win), w->textview);

	hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5 );
	gtk_box_pack_start(GTK_BOX(hbox), w->entry, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox), insert, FALSE, FALSE, 5);

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}