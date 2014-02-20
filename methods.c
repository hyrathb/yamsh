#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "defines.h"
#include "structs.h"
#include "methods.h"


/***********************************************
gtk+ & system
***********************************************/
void create_main()
{
    GtkWidget *menubar;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkWidget *type_popup_menu;
    GtkWidget *menu_item;


    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "客房信息管理系统");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    /* Create a vertical box*/
    vbox= gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);


    /*Create the main menu*/
    menubar = gtk_menu_bar_new();
    create_menu(win, menubar);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);


    /*the label*/
    label= gtk_label_new("客房类型信息一览表");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);


    /*Create the tree view of types*/
    liststore= gtk_list_store_new(COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(treeview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("客房类型", render, "text", TYPE_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("最多容纳人数", render, "text", MOST_IN_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("总客房数", render, "text", TOTAL_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("剩余客房数", render, "text", LEFT_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("入住数", render, "text", IN_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column5);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    type_popup_menu= gtk_menu_new();
    menu_item= gtk_menu_item_new_with_label("查看详情");
    g_signal_connect(menu_item, "activate", type_view_details, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("编辑");
    g_signal_connect(menu_item, "activate", handle_edit_type_dialogue, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("删除");
    g_signal_connect(menu_item, "activate", handle_type_delete, NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(type_popup_menu), menu_item);

    struct popup_menu *type_menu= (struct popup_menu *)malloc(sizeof(struct popup_menu));
    type_menu->menu= type_popup_menu;
    type_menu->treeview= GTK_TREE_VIEW(treeview);
    type_menu->selection= selection;
    type_menu->active= (void **)&active_type;

    g_signal_connect_swapped(treeview, "button_press_event", G_CALLBACK(handle_popup_menu), type_menu);
    g_signal_connect(treeview, "row-activated", G_CALLBACK(handle_type_row_activated), selection);


    gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all (win);

}

void create_room_win()
{
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *roomview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;
    GtkWidget *room_popup_menu;
    GtkWidget *menu_item;


    room_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (room_win), 8);
    gtk_window_set_title (GTK_WINDOW (room_win), "客房类型详情");
    gtk_window_set_position (GTK_WINDOW (room_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (room_win);
    gtk_window_set_transient_for(GTK_WINDOW(room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(room_win), TRUE);

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    gtk_container_add (GTK_CONTAINER (room_win), box1);

    roomstore= gtk_list_store_new(ROOM_COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_STRING);
    roomview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(roomstore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(roomview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("房间号", render, "text", ROOM_NUMBER_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("电话号码", render, "text", PHONE_NUMBER_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("面积", render, "text", AREA_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("价格", render, "text", PRICE_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("空闲", render, "text", IS_LIFT_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(roomview), column5);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(roomview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    button1= gtk_button_new_with_label("新建");
    button2= gtk_button_new_with_label("编辑");
    button3= gtk_button_new_with_label("查看详情");
    button4= gtk_button_new_with_label("删除");
    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button4, TRUE, TRUE, 0);

    struct room_dialogue_check *data= (struct room_dialogue_check *)malloc(sizeof(struct room_dialogue_check));
    data->button1= button2;
    data->button2= button3;
    data->button3= button4;

    room_popup_menu= gtk_menu_new();
    menu_item= gtk_menu_item_new_with_label("查看详情");
    g_signal_connect(menu_item, "activate", G_CALLBACK(room_view_detail), NULL);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("编辑");
    g_signal_connect(menu_item, "activate", G_CALLBACK(handle_room_edit_assistant), data);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);
    menu_item= gtk_menu_item_new_with_label("删除");
    g_signal_connect(menu_item, "activate", G_CALLBACK(handle_room_delete), NULL);
    g_signal_connect(menu_item, "activate", G_CALLBACK(room_dialogue_check), data);
    gtk_widget_show(menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(room_popup_menu), menu_item);

    struct popup_menu *room_menu= (struct popup_menu *)malloc(sizeof(struct popup_menu));
    room_menu->menu= room_popup_menu;
    room_menu->treeview= GTK_TREE_VIEW(roomview);
    room_menu->selection= selection;
    room_menu->active= (void **)&active_room;

    g_signal_connect_swapped(roomview, "button_press_event", G_CALLBACK(handle_popup_menu), room_menu);
    g_signal_connect(roomview, "row-activated", G_CALLBACK(handle_room_row_activated), selection);

    gtk_box_pack_start(GTK_BOX(box1), roomview, TRUE, TRUE, 0);

    g_signal_connect(button2, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_room_find_focus), selection);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_room_new_assistant), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_room_edit_assistant), data);
    g_signal_connect(button3, "clicked", G_CALLBACK(handle_room_view_detail), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_room_delete), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(room_dialogue_check), data);
    g_signal_connect(room_win, "show", G_CALLBACK(room_dialogue_check), data);

    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    g_signal_connect(room_win, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), room_win);

}

void create_guest_win()
{
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *guestview;
    GtkTreeSelection *selection;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkTreeViewColumn *column7;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;


    guest_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (guest_win), 8);
    gtk_window_set_position (GTK_WINDOW (guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(guest_win), TRUE);

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    gtk_container_add (GTK_CONTAINER (guest_win), box1);

    gueststore= gtk_list_store_new(GUEST_COLUMN_NUMBER, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    guestview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(gueststore));
    //gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(guestview), FALSE);
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);

    column1= gtk_tree_view_column_new_with_attributes("身份证号", render, "text", GUEST_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("姓名", render, "text", GUEST_NAME_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("入住时间", render, "text", GUEST_IN_TIME_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("退房时间", render, "text", GUEST_OUT_TIME_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("总计时间", render, "text", GUEST_DAY_SPENT_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("应付费用", render, "text", GUEST_FARE_COLUMN, NULL);
    column7= gtk_tree_view_column_new_with_attributes("实付费用", render, "text", GUEST_PAID_COLUMN, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column6);
    gtk_tree_view_append_column(GTK_TREE_VIEW(guestview), column7);

    selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(guestview));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    gtk_box_pack_start(GTK_BOX(box1), guestview, TRUE, TRUE, 0);

    button1= gtk_button_new_with_label("新入住");
    button2= gtk_button_new_with_label("编辑");
    button3= gtk_button_new_with_label("新退房");
    button4= gtk_button_new_with_label("删除");
    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button4, TRUE, TRUE, 0);

    struct guest_dialogue_check *data= (struct guest_dialogue_check *)malloc(sizeof(struct guest_dialogue_check));
    data->button1= button2;
    data->button2= button4;

    g_signal_connect(button2, "clicked", G_CALLBACK(handle_guest_find_focus), selection);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_guest_find_focus), selection);
    g_signal_connect_swapped(button1, "clicked", G_CALLBACK(guest_in_dialogue), data);
    g_signal_connect_swapped(button2, "clicked", G_CALLBACK(guest_modify_dialogue), data);
    g_signal_connect(button3, "clicked", G_CALLBACK(guest_out_dialogue), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(handle_guest_delete), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(guest_dialogue_check), data);
    g_signal_connect(guest_win, "show", G_CALLBACK(guest_dialogue_check), data);


    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    g_signal_connect(guest_win, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), guest_win);
}


void (*handle[])()= {file_new, file_open, file_save, file_save_as, file_close, file_quit, edit_new_type, edit_new_room, edit_new_guest_in, edit_new_guest_out, query_room, query_guest, sta_by_month, sta_by_year, sta_by_guest, sta_by_room, help_about, help_help};

void errmesg(const char *error)
{
    GtkWidget *dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, error);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void handle_destroy_dialogue(GtkWidget *ignored, GtkWidget *dialogue)
{
    gtk_widget_destroy(dialogue);
}

void handle_dialogue_hide(GtkWidget *ignored, GtkWidget *dialogue)
{
    gtk_widget_hide(dialogue);
}

void free_data(GtkWidget *ignored, void *data)
{
    free(data);
}

int handle_popup_menu(struct popup_menu *pop_struct, GdkEvent *event)
{
    if (!file)
        return FALSE;
    GtkMenu *menu;
    GdkEventButton *event_button;

    g_return_val_if_fail (pop_struct->menu != NULL, FALSE);
    g_return_val_if_fail (GTK_IS_MENU (pop_struct->menu), FALSE);
    g_return_val_if_fail (event != NULL, FALSE);

    if (event->type == GDK_BUTTON_PRESS)
    {
        event_button = (GdkEventButton *) event;
        GtkTreePath *path;
        if (gtk_tree_view_is_blank_at_pos(GTK_TREE_VIEW(pop_struct->treeview), event_button->x, event_button->y, &path, NULL, NULL, NULL))
            return FALSE;
        gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(pop_struct->treeview), event_button->x, event_button->y, &path, NULL, NULL, NULL);
        gtk_tree_model_get_iter(gtk_tree_view_get_model(pop_struct->treeview), &treeiter, path);
        gtk_tree_selection_select_iter(pop_struct->selection, &treeiter);
        gtk_tree_model_get(gtk_tree_view_get_model(pop_struct->treeview), &treeiter, 0, pop_struct->active, -1);
        menu= GTK_MENU(pop_struct->menu);
        if (event_button->button == GDK_BUTTON_SECONDARY)
        {
            gtk_menu_popup (menu, NULL, NULL, NULL, NULL, event_button->button, event_button->time);
            return TRUE;
        }
    }

    return FALSE;
}

void handle_room_find_focus(GtkWidget *ignored, GtkTreeSelection *selection)
{
    gtk_tree_selection_get_selected(selection,(GtkTreeModel **)(&roomstore), &treeiter);
    if (treeiter.stamp== 0)
    {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(roomstore), &treeiter);
        gtk_tree_selection_select_iter(selection, &treeiter);
    }
    gtk_tree_model_get(GTK_TREE_MODEL(roomstore), &treeiter, ROOM_POINTER_COLUMN, &active_room);
}

void handle_new_type(GtkWidget *ignored, struct new_type_data *data)
{
    char type[20];
    int most_in;
    strcpy(type, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    most_in= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    free(data);
    type_insert(type, most_in);
    gtk_list_store_insert_with_values(liststore, &treeiter, 0,
                                      POINTER_COLUMN, (gpointer)type_head,
                                      TYPE_COLUMN, type,
                                      MOST_IN_COLUMN, most_in,
                                      TOTAL_COLUMN, type_head->total_,
                                      LEFT_COLUMN, type_head->left_,
                                      IN_COLUMN, type_head->total_-type_head->left_,
                                      -1);
    edited= 1;
}


void handle_type_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection)
{
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&liststore, &treeiter);
    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &treeiter, POINTER_COLUMN, &active_type, -1);
    type_view_details();
}

void handle_edit_type_dialogue()
{
    type_dialogue(EDIT);
}

void handle_edit_type(GtkWidget *ignored, struct new_type_data *data)
{
    char type[20];
    int most_in;
    strcpy(type, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    most_in= atoi(gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    free(data);
    type_modify(type, most_in);
    gtk_list_store_set(liststore, &treeiter,
                       POINTER_COLUMN, (gpointer)active_type,
                       TYPE_COLUMN, type,
                       MOST_IN_COLUMN, most_in,
                       TOTAL_COLUMN, active_type->total_,
                       LEFT_COLUMN, active_type->left_,
                       IN_COLUMN, active_type->total_- active_type->left_,
                       -1);
    edited= 1;
}

void handle_type_delete()
{
    gtk_list_store_remove(liststore, &treeiter);
    type_delete();
}

void handle_room_new_assistant(void *ignored, struct room_dialogue_check *data)
{
    room_dialogue(NEW, data);
}

void handle_room_edit_assistant(void *ignored, struct room_dialogue_check *data)
{
    room_dialogue(EDIT, data);
}

void handle_room_new(GtkWidget *ignored, struct new_room_data *data)
{

    struct basic_room_type_data *type;
    char room_number[6];
    char phone_number[20];
    float area;
    float price;
    int is_left;

    GtkTreeIter iter;
    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(data->combo1)), &iter, POINTER_COLUMN, &type, -1);
    strcpy(room_number, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    strcpy(phone_number, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    area= atof(gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    price= atof(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    is_left= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton));

    free(data);

    room_insert(type, room_number, phone_number, area, price, is_left);

    GtkTreeIter titer;
    GtkTreePath *path= gtk_tree_path_new_first();
    struct basic_room_type_data *tmp;
    gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, POINTER_COLUMN, &tmp, -1);
    while (tmp!=type)
    {
        gtk_tree_path_next(path);
        gtk_tree_model_get_iter(GTK_TREE_MODEL(liststore), &titer, path);
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &titer, POINTER_COLUMN, &tmp, -1);
    }
    gtk_list_store_set(liststore, &titer,
                       POINTER_COLUMN, (gpointer) type,
                       TYPE_COLUMN, type->type_,
                       MOST_IN_COLUMN, type->most_in_,
                       TOTAL_COLUMN, type->total_,
                       LEFT_COLUMN, type->left_,
                       IN_COLUMN, type->total_- type->left_,
                       -1);
    if (gtk_widget_get_visible(room_win))
    {
        gtk_list_store_clear(roomstore);
        access_room_data();
    }
}


void handle_room_modify(GtkWidget *ignored, struct new_room_data *data)
{
    struct basic_room_type_data *type;
    char room_number[6];
    char phone_number[20];
    float area;
    float price;
    int is_left;

    if ((active_room==NULL) || (active_room->type_!= active_type))
        return;

    GtkTreeIter iter;
    gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(data->combo1)), &iter, POINTER_COLUMN, &type, -1);
    strcpy(room_number, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    strcpy(phone_number, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    area= atof(gtk_entry_get_text(GTK_ENTRY(data->entry3)));
    price= atof(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    is_left= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton));

    free(data);

    room_modify(type, room_number, phone_number, area, price, is_left);

    gtk_list_store_clear(roomstore);
    access_room_data();

    gtk_list_store_clear(liststore);
    access_type_data();
}

void handle_room_delete()
{
    if ((active_room==NULL) || (active_room->type_!= active_type))
        return;

    gtk_list_store_remove(GTK_LIST_STORE(roomstore), &treeiter);

    room_delete();

    gtk_list_store_clear(liststore);
    access_type_data();
}

void handle_room_view_detail()
{
    room_view_detail();
}

void handle_room_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, GtkTreeSelection* selection)
{
    GtkTreeIter iter;
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&roomstore, &iter);
    gtk_tree_model_get(GTK_TREE_MODEL(roomstore), &iter, ROOM_POINTER_COLUMN, &active_room, -1);
    room_view_detail();
}

void handle_guest_find_focus(GtkWidget *ignored, GtkTreeSelection *selection)
{
    gtk_tree_selection_get_selected(selection, (GtkTreeModel **)&gueststore, &treeiter);
    if (treeiter.stamp== 0)
    {
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(gueststore), &treeiter);
        gtk_tree_selection_select_iter(selection, &treeiter);
    }
    gtk_tree_model_get(GTK_TREE_MODEL(gueststore), &treeiter, GUEST_POINTER_COLUMN, &active_guest);
}


void handle_guest_in_new(GtkWidget *ignored, struct new_guest_in_data *data)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    int con= 1;
    while (type_iter && con)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {

            if (!(con =strcmp(room_iter->room_number_, gtk_entry_get_text(GTK_ENTRY(data->entry1)))))
                break;
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    if (!room_iter->is_left_)
        errmesg("The room is in use.");
    else if (con)
        errmesg("No such room.");
    else
    {
        char id[20];
        char name[20];
        char in_time[18];
        strcpy(name, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        strcpy(in_time, gtk_entry_get_text(GTK_ENTRY(data->entry4)));

        free(data);

        guest_in_insert(room_iter, id, name, in_time);

        gtk_list_store_clear(liststore);
        access_type_data();

        if (gtk_widget_get_visible(room_win))
        {
            gtk_list_store_clear(GTK_LIST_STORE(roomstore));
            access_room_data();
        }

        if (gtk_widget_get_visible(guest_win))
        {
            gtk_list_store_clear(GTK_LIST_STORE(gueststore));
            access_guest_data();
        }
    }
}

void handle_guest_out_new(GtkWidget *ignored, struct new_guest_out_data *data)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    int con= 1;
    while (type_iter && con)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {

            if (!(con =strcmp(room_iter->room_number_, gtk_entry_get_text(GTK_ENTRY(data->entry1)))))
                break;
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    if (con)
        errmesg("No such room.");
    else
    {
        char id[20];
        char out_time[18];
        float pay;
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
        strcpy(out_time, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        pay= atof(gtk_entry_get_text(GTK_ENTRY(data->entry4)));

        free(data);

        struct basic_guest_info *guest_iter= room_iter->guest_;
        while (guest_iter)
        {
            if (!(strcmp(guest_iter->id_, id) && (guest_iter->out_time_[0]==0)))
                break;
            guest_iter= guest_iter->next_;
        }
        if (!guest_iter)
        {
            errmesg("No guest in info.");
            return;
        }

        guest_out_insert(guest_iter, out_time, pay);

        gtk_list_store_clear(liststore);
        access_type_data();

        if (gtk_widget_get_visible(room_win))
        {
            gtk_list_store_clear(GTK_LIST_STORE(roomstore));
            access_room_data();
        }

        if (gtk_widget_get_visible(guest_win))
        {
            gtk_list_store_clear(GTK_LIST_STORE(gueststore));
            access_guest_data();
        }
    }
}

void handle_guest_modify(GtkWidget *ignored, struct new_guest_data *data)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    int con= 1;
    while (type_iter && con)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {

            if (!(con =strcmp(room_iter->room_number_, gtk_entry_get_text(GTK_ENTRY(data->entry1)))))
                break;
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    if (con)
        errmesg("No such room.");
    else
    {
        char name[20];
        char id[20];
        char in_time[18];
        char out_time[18];
        float pay;

        strcpy(name, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        strcpy(in_time, gtk_entry_get_text(GTK_ENTRY(data->entry4)));
        strcpy(out_time, gtk_entry_get_text(GTK_ENTRY(data->entry5)));
        pay= atof(gtk_entry_get_text(GTK_ENTRY(data->entry6)));

        free(data);

        guest_modify(room_iter, id, name, in_time, out_time, pay);

        gtk_list_store_clear(liststore);
        access_type_data();

        gtk_list_store_clear(GTK_LIST_STORE(roomstore));
        access_room_data();

        gtk_list_store_clear(GTK_LIST_STORE(gueststore));
        access_guest_data();
    }


}

void handle_guest_delete()
{
    if ((active_guest==NULL) || (active_guest->room_info_!= active_room))
        return;
    gtk_list_store_remove(gueststore, &treeiter);

    guest_delete();

    gtk_list_store_clear(roomstore);
    access_room_data();
    gtk_list_store_clear(liststore);
    access_type_data();
}

void handle_query_room(GtkWidget *ignored, struct query_room *data)
{
    char room_num[20];
    char phone_num[20];
    struct basic_room_type_data *type;
    int is_type_limited;
    int is_room_number_limited;
    int is_phone_number_limited;
    int is_left;
    int is_left_limited;
    float uprice;
    float lprice;
    int is_price_limited;
    float uarea;
    float larea;
    int is_area_limited;

    is_room_number_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton1));
    is_phone_number_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton2));
    is_type_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton3));
    is_left_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton4));
    is_price_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton5));
    is_area_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton6));

    if (is_room_number_limited)
        strcpy(room_num, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    if (is_phone_number_limited)
        strcpy(phone_num, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    if (is_type_limited)
    {
        GtkTreeIter iter;
        gtk_combo_box_get_active_iter(GTK_COMBO_BOX(data->combo1), &iter);
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, POINTER_COLUMN, &type, -1);
    }
    if (is_left_limited)
        is_left= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton7));
    if (is_price_limited)
    {
        lprice= atof(gtk_entry_get_text(GTK_ENTRY(data->entry4)));
        uprice= atof(gtk_entry_get_text(GTK_ENTRY(data->entry5)));
    }
    if (is_area_limited)
    {
        larea= atof(gtk_entry_get_text(GTK_ENTRY(data->entry6)));
        uarea= atof(gtk_entry_get_text(GTK_ENTRY(data->entry7)));
    }

    q_room_by_conditions(data->store, is_room_number_limited, is_phone_number_limited, is_type_limited, is_left_limited, is_price_limited, is_area_limited, type, room_num, phone_num, is_left, uprice, lprice, uarea, larea);
}

void handle_query_guest(GtkWidget *ignored, struct query_guest *data)
{
    char id[20];
    char name_piece[20];
    char lin_time[18];
    char uin_time[18];
    int is_id_limited;
    int is_name_limited;
    int is_in_time_limited;

    is_id_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton1));
    is_name_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton2));
    is_in_time_limited= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->checkbutton3));

    if (is_id_limited)
        strcpy(id, gtk_entry_get_text(GTK_ENTRY(data->entry1)));
    if (is_name_limited)
        strcpy(name_piece, gtk_entry_get_text(GTK_ENTRY(data->entry2)));
    if (is_in_time_limited)
    {
        strcpy(lin_time, gtk_entry_get_text(GTK_ENTRY(data->entry3)));
        strcpy(uin_time, gtk_entry_get_text(GTK_ENTRY(data->entry4)));
    }

    q_guest_info_by_conditions(data->store, is_id_limited, is_name_limited, is_in_time_limited, id, name_piece, lin_time, uin_time);

}

void handle_sta_by_year(GtkWidget *ignored, GtkEntry *entry)
{
    int year= atoi(gtk_entry_get_text(entry));

    GtkWidget *sta_by_year_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *treview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column;

    sta_by_year_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_by_year_win), 8);
    gtk_window_set_position (GTK_WINDOW (sta_by_year_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_by_year_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_by_year_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_by_year_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_by_year_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_by_year_win), "房间状况统计");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_by_year_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    char y[15];
    sprintf(y, "统计时间：%d", year);
    label1= gtk_label_new(y);
    label2= gtk_label_new("单位:元人民币");

    store= gtk_list_store_new(STA_ROOM_COLUMN_NUMBER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    treview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("房间编号", render, "text", STA_ROOM_NUMBER_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);
    column= gtk_tree_view_column_new_with_attributes("房间类别", render, "text", STA_ROOM_TYPE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);
    column= gtk_tree_view_column_new_with_attributes("营业额", render, "text", STA_W_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);
    column= gtk_tree_view_column_new_with_attributes("入住天数", render, "text", STA_IN_TIME_COLUME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);
    column= gtk_tree_view_column_new_with_attributes("入住率", render, "text", STA_IN_REAL_CULUME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treview), column);

    cal_rooms_sta(store, year);

    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), label2, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_by_year_win);
}

void room_dialogue_check(GtkMenuItem *ignored, struct room_dialogue_check *data)
{
    if (active_type && active_type->room_head_)
    {
        gtk_widget_set_sensitive(data->button1, TRUE);
        gtk_widget_set_sensitive(data->button2, TRUE);
        gtk_widget_set_sensitive(data->button3, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(data->button1, FALSE);
        gtk_widget_set_sensitive(data->button2, FALSE);
        gtk_widget_set_sensitive(data->button3, FALSE);
    }
}

void guest_dialogue_check(GtkMenuItem *ignored, struct guest_dialogue_check *data)
{
    if (active_type && active_room  && active_room->guest_)
    {
        gtk_widget_set_sensitive(data->button1, TRUE);
        gtk_widget_set_sensitive(data->button2, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(data->button1, FALSE);
        gtk_widget_set_sensitive(data->button2, FALSE);
    }
}

void file_close(GtkWidget *win)
{
    if (file)
    {
        gtk_list_store_clear(liststore);
        fclose(file);
        file= NULL;
    }
}


void file_new(GtkWidget *win)
{
    char *filename= choose_file(SAVE_FILE);
    if (filename)
    {
        file_close(win);
        remove(filename);
        file= fopen(filename, "wb+");
        free(filename);
        if (!file)
        {
            errmesg("Access Denied.");
        }
    }

}

void file_open(GtkWidget *win)
{
    char *filename= choose_file(OPEN_FILE);
    if (filename)
    {
        file_close(win);
        file= fopen(filename, "rb+");
        if (!file)
        {
            errmesg("Access Denied.");
        }
        else
        {
            type_head= load_data();
            access_type_data();
        }
        free(filename);
    }
}

void file_save(GtkWidget *win)
{
    save_data();
}

void file_save_as(GtkWidget *win)
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    char *filename= choose_file(SAVE_FILE_AS);
    if (filename)
    {
        FILE *tfile;
        tfile= file;
        remove(filename);
        file= fopen(filename, "wb+");
        free(filename);
        if (!file)
        {
            errmesg("Access Denied.");
        }
        else
        {
            save_data();
        }
        fclose(file);
        file= tfile;
    }
}

void file_quit(GtkWidget *win)
{
    file_close(win);
    gtk_main_quit();
}

void edit_new_type(GtkWidget *win)
{
    if (file)
        type_dialogue(NEW);
    else
        errmesg("No File Opened.");
}

void edit_new_room(GtkWidget *win)
{
    if (file)
    {
        if (type_head)
            room_dialogue(NEW, NULL);
        else
            errmesg("No room type available.");
    }
    else
        errmesg("No File Opened.");
}

void edit_new_guest_in(GtkWidget *win)
{
    if (file)
        guest_in_dialogue(NULL);
}

void edit_new_guest_out(GtkWidget *win)
{
    if (file)
        guest_out_dialogue();
}

void query_room()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }

    GtkWidget *query_room_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *box4;
    GtkWidget *grid;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *label7;
    GtkWidget *label8;
    GtkWidget *label9;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *entry7;
    GtkWidget *combo1;
    GtkCellRenderer *listrender;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton7;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkWidget *checkbutton4;
    GtkWidget *checkbutton5;
    GtkWidget *checkbutton6;
    GtkWidget *button1;
    GtkWidget *button2;

    query_room_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (query_room_win), 8);
    gtk_window_set_position (GTK_WINDOW (query_room_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (query_room_win);
    gtk_window_set_transient_for(GTK_WINDOW(query_room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(query_room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(query_room_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(query_room_win), "查询");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(query_room_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    box4= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box4), TRUE);

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_column(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_grid_insert_row(GTK_GRID(grid), 6);

    label1= gtk_label_new("按条件查找房间");

    label2= gtk_label_new("房间号");
    entry1= gtk_entry_new();
    checkbutton1= gtk_check_button_new_with_label("限定");

    label3= gtk_label_new("电话号");
    entry2= gtk_entry_new();
    checkbutton2= gtk_check_button_new_with_label("限定");

    label4=gtk_label_new("房间类型");
    combo1= gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo1), TYPE_COLUMN);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1), 0);
    listrender= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo1), listrender, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo1), listrender, "text", TYPE_COLUMN, NULL);
    checkbutton3= gtk_check_button_new_with_label("限定");

    label5= gtk_label_new("是否空闲");
    checkbutton7= gtk_check_button_new_with_label("空闲");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton7), TRUE);
    checkbutton4= gtk_check_button_new_with_label("限定");

    label6= gtk_label_new("价格");
    entry4= gtk_entry_new();
    label8= gtk_label_new("至");
    entry5= gtk_entry_new();
    checkbutton5= gtk_check_button_new_with_label("限定");

    label7= gtk_label_new("面积");
    entry6= gtk_entry_new();
    label9= gtk_label_new("至");
    entry7= gtk_entry_new();
    checkbutton6= gtk_check_button_new_with_label("限定");

    gtk_box_pack_start(GTK_BOX(box2), entry4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), label8, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), entry5, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box3), entry6, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), label9, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry7, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo1, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton3, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton7, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton4, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label6, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box2, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton5, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label7, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box3, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton6, 2, 6, 1, 1);

    store= gtk_list_store_new(ROOM_COLUMN_NUMBER+1, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_STRING, G_TYPE_STRING);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("房间号", render, "text", ROOM_NUMBER_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("电话号码", render, "text", PHONE_NUMBER_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("面积", render, "text", AREA_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("价格", render, "text", PRICE_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("空闲", render, "text", IS_LIFT_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("房间类型", render, "text", ROOM_COLUMN_NUMBER, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column6);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column5);

    button1= gtk_button_new_with_label("查询");
    button2= gtk_button_new_with_label("关闭");

    gtk_box_pack_start(GTK_BOX(box4), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box4), button2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treeview, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box4, FALSE, TRUE, 0);

    struct query_room *data= (struct query_room *)malloc(sizeof(struct query_room));
    data->entry1= entry1;
    data->entry2= entry2;
    data->combo1= combo1;
    data->entry4= entry4;
    data->entry5= entry5;
    data->entry6= entry6;
    data->entry7= entry7;
    data->checkbutton1= checkbutton1;
    data->checkbutton2= checkbutton2;
    data->checkbutton3= checkbutton3;
    data->checkbutton4= checkbutton4;
    data->checkbutton5= checkbutton5;
    data->checkbutton6= checkbutton6;
    data->checkbutton7= checkbutton7;
    data->store= store;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_query_room), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(free_data), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), query_room_win);

    gtk_widget_show_all(query_room_win);
}

void query_guest()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }

    GtkWidget *query_guest_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *grid;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkCellRenderer *render;
    GtkTreeViewColumn *column1;
    GtkTreeViewColumn *column2;
    GtkTreeViewColumn *column3;
    GtkTreeViewColumn *column4;
    GtkTreeViewColumn *column5;
    GtkTreeViewColumn *column6;
    GtkTreeViewColumn *column7;
    GtkTreeViewColumn *column8;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton2;
    GtkWidget *checkbutton3;
    GtkWidget *button1;
    GtkWidget *button2;

    query_guest_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (query_guest_win), 8);
    gtk_window_set_position (GTK_WINDOW (query_guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (query_guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(query_guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(query_guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(query_guest_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(query_guest_win), "查询");

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(query_guest_win), box1);
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box2), TRUE);

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_column(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);

    label1= gtk_label_new("查询住宿信息");

    label2= gtk_label_new("身份证号");
    entry1= gtk_entry_new();
    checkbutton1= gtk_check_button_new_with_label("限定");

    label3= gtk_label_new("姓名或姓名片段");
    entry2= gtk_entry_new();
    checkbutton2= gtk_check_button_new_with_label("限定");

    label4=gtk_label_new("入住时间段");
    entry3= gtk_entry_new();
    label5= gtk_label_new("至");
    entry4=gtk_entry_new();
    box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    gtk_box_pack_start(GTK_BOX(box3), entry3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), label5, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry4, TRUE, TRUE, 0);
    checkbutton3= gtk_check_button_new_with_label("限定");

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton3, 2, 3, 1, 1);

    store= gtk_list_store_new(GUEST_COLUMN_NUMBER+1, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_STRING);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column1= gtk_tree_view_column_new_with_attributes("身份证号", render, "text", GUEST_ID_COLUMN, NULL);
    column2= gtk_tree_view_column_new_with_attributes("姓名", render, "text", GUEST_NAME_COLUMN, NULL);
    column3= gtk_tree_view_column_new_with_attributes("入住时间", render, "text", GUEST_IN_TIME_COLUMN, NULL);
    column4= gtk_tree_view_column_new_with_attributes("退房时间", render, "text", GUEST_OUT_TIME_COLUMN, NULL);
    column5= gtk_tree_view_column_new_with_attributes("总计时间", render, "text", GUEST_DAY_SPENT_COLUMN, NULL);
    column6= gtk_tree_view_column_new_with_attributes("应付费用", render, "text", GUEST_FARE_COLUMN, NULL);
    column7= gtk_tree_view_column_new_with_attributes("实付费用", render, "text", GUEST_PAID_COLUMN, NULL);
    column8= gtk_tree_view_column_new_with_attributes("所住房间", render, "text", GUEST_COLUMN_NUMBER, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column8);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column6);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column7);


    button1= gtk_button_new_with_label("查询");
    button2= gtk_button_new_with_label("关闭");

    gtk_box_pack_start(GTK_BOX(box2), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button2, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), treeview, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    struct query_guest *data= (struct query_guest *)malloc(sizeof(struct query_guest));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->checkbutton1= checkbutton1;
    data->checkbutton2= checkbutton2;
    data->checkbutton3= checkbutton3;
    data->store= store;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_query_guest), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(free_data), data);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), query_guest_win);

    gtk_widget_show_all(query_guest_win);
}

void sta_by_month()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    GtkWidget *sta_month_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_month_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_month_win), 8);
    gtk_window_set_position (GTK_WINDOW (sta_month_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_month_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_month_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_month_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_month_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_month_win), "营业额统计");

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_month_win), box);

    time_t now;
    int year;
    char stime[50];
    const char *p= stime;
    time(&now);
    strftime(stime, 50, "统计时间: %Y 年%m月%d日%H时%M分%S秒", localtime(&now));
    get_num(&p, ' ');
    year=get_num(&p, ' ');

    label= gtk_label_new(stime);

    int types= cal_types();
    GType type_list[types];
    fill_type(types, type_list);
    store= gtk_list_store_newv(types+1, type_list);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    int i;
    struct basic_room_type_data *type_iter= type_head;
    column= gtk_tree_view_column_new_with_attributes("月份", render, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    for (i=1; i<= types; i++, type_iter= type_iter->next_)
    {
        column= gtk_tree_view_column_new_with_attributes(type_iter->type_, render, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    GtkTreeIter iter;
    GValue value[types+1];
    float total[types+1];
    memset(total, 0, sizeof(total));
    int column_num[types+1];
    for (i=1; i<=12; i++)
    {
        memset(value, 0, sizeof(value));
        cal_values(types, year, i, column_num, value, total);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set_valuesv(store, &iter, column_num, value, types+1);
    }
    memset(value, 0, sizeof(value));
    g_value_init(value, G_TYPE_STRING);
    g_value_set_string(value, "总计");
    for (i=1; i<=types; i++)
    {
        g_value_init(value+i, G_TYPE_FLOAT);
        g_value_set_float(value+i, total[i]);
    }
    gtk_list_store_append(store, &iter);
    gtk_list_store_set_valuesv(store, &iter, column_num, value, types+1);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_month_win);
}

void sta_by_year()
{
    if (!file)
    {
        errmesg("No File Opened.");
        return;
    }
    GtkWidget *sta_year_win;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *button;

    sta_year_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_year_win), 0);
    gtk_window_set_position (GTK_WINDOW (sta_year_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_year_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_year_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_year_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_year_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_year_win), "营业额统计");
    gtk_window_resize(GTK_WINDOW(sta_year_win), 4, 2);

    box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_year_win), box1);

    label= gtk_label_new("请输入查询年份:");

    entry= gtk_entry_new();
    button= gtk_button_new_with_label("确定");
    box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_pack_start(GTK_BOX(box2), entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box1), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, TRUE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(handle_sta_by_year), entry);
    g_signal_connect(button, "clicked", G_CALLBACK(handle_destroy_dialogue), sta_year_win);

    gtk_widget_show_all(sta_year_win);

}

void sta_by_guest()
{
    GtkWidget *sta_guest_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_guest_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_guest_win), 0);
    gtk_window_set_position (GTK_WINDOW (sta_guest_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_guest_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_guest_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_guest_win), "旅客统计");
    gtk_window_resize(GTK_WINDOW(sta_guest_win), 4, 2);

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_guest_win), box);

    label= gtk_label_new("Top 10旅客:");

    store= gtk_list_store_new(STA_GUEST_COLUMN_NUMBER, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("排名", render, "text", STA_GUEST_RANK_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("身份证号", render, "text", STA_GUEST_ID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("旅客姓名", render, "text", STA_GUEST_NAME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("累计住宿天数", render, "text", STA_GUEST_IN_TIME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("应缴费用总额", render, "text", STA_GUEST_FARE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("实缴费用总额", render, "text", STA_GUEST_PAID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("折扣率", render, "text", STA_DISCOUNT_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    cal_top_10_guest(store);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_guest_win);

}

void sta_by_room()
{
    GtkWidget *sta_room_win;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *render;

    sta_room_win= gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (sta_room_win), 0);
    gtk_window_set_position (GTK_WINDOW (sta_room_win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (sta_room_win);
    gtk_window_set_transient_for(GTK_WINDOW(sta_room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(sta_room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(sta_room_win), TRUE);
    gtk_window_set_title(GTK_WINDOW(sta_room_win), "客房统计");
    gtk_window_resize(GTK_WINDOW(sta_room_win), 4, 2);

    box= gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(sta_room_win), box);

    label= gtk_label_new("客房盈利排序:");

    store= gtk_list_store_new(STA_ROOM_COLUMN_NUMBER2, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);
    treeview= gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    render= gtk_cell_renderer_text_new();
    g_object_set(G_OBJECT(render), "foreground", "black", NULL);
    column= gtk_tree_view_column_new_with_attributes("排名", render, "text", STA_ROOM_RANK_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("房间号", render, "text", STA_ROOM_NUMBER_COLUMN2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("客房类型", render, "text", STA_ROOM_TYPE_COLUMN2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("应缴费用总额", render, "text", STA_ROOM_FARE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("实缴费用总额", render, "text", STA_ROOM_PAID_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    column= gtk_tree_view_column_new_with_attributes("折扣率", render, "text", STA_ROOM_DISCOUNT_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    cal_top_room(store);

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), treeview, TRUE, TRUE, 0);

    gtk_widget_show_all(sta_room_win);
}

void help_about()
{
    GtkWidget *about_win;
    about_win= gtk_about_dialog_new();
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_win), version);
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_win), "yamsh");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(about_win), GTK_LICENSE_GPL_2_0);
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(about_win), url);
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG(about_win), "github:https://github.com/hyrathb/yamsh");
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(about_win), auther);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_win), NULL);
    gtk_widget_show_all(about_win);
}

void help_help(GtkWidget *win)
{
    errmesg("暂无.");
}

void access_type_data()
{
    struct basic_room_type_data *type_itor= type_head;
    while (type_itor)
    {
        gtk_list_store_insert_with_values(liststore, &treeiter, -1,
                                          POINTER_COLUMN, (gpointer)type_itor,
                                          TYPE_COLUMN, type_itor->type_,
                                          MOST_IN_COLUMN, type_itor->most_in_,
                                          TOTAL_COLUMN, type_itor->total_,
                                          LEFT_COLUMN, type_itor->left_,
                                          IN_COLUMN, type_itor->total_- type_itor->left_,
                                          -1);
        type_itor= type_itor->next_;
    }
}

void access_room_data()
{
    struct basic_room_data *room_iter= active_type->room_head_;
    GtkTreeIter iter;
    while (room_iter)
    {
        gtk_list_store_insert_with_values(roomstore, &iter, -1,
                                          ROOM_POINTER_COLUMN, room_iter,
                                          ROOM_NUMBER_COLUMN, room_iter->room_number_,
                                          PHONE_NUMBER_COLUMN, room_iter->phone_number_,
                                          AREA_COLUMN, room_iter->area_,
                                          PRICE_COLUMN, room_iter->price_,
                                          IS_LIFT_COLUMN, room_iter->is_left_? "是": "否",
                                          -1);
        room_iter= room_iter->next_;
    }
}

void access_guest_data()
{
    struct basic_guest_info *guest_iter= active_room->guest_;
    GtkTreeIter iter;
    while (guest_iter)
    {
        gtk_list_store_insert_with_values(gueststore, &iter, -1,
                                          GUEST_POINTER_COLUMN, guest_iter,
                                          GUEST_ID_COLUMN, guest_iter->id_,
                                          GUEST_NAME_COLUMN, guest_iter->name_,
                                          GUEST_IN_TIME_COLUMN, guest_iter->in_time_,
                                          GUEST_OUT_TIME_COLUMN, (guest_iter->out_time_[0]==0)?"在住":guest_iter->out_time_,
                                          GUEST_DAY_SPENT_COLUMN, guest_iter->day_spent_,
                                          GUEST_FARE_COLUMN, guest_iter->fare_,
                                          GUEST_PAID_COLUMN, guest_iter->pay_);
        guest_iter= guest_iter->next_;
    }
}

void create_menu(GtkWidget *win, GtkWidget *menubar)
{
    int i, j, k=0;
    char **strings= (char **)menu_string;
    for (i= 0; i< MENU_NUM; i++)
    {
        GtkWidget *menu_item= gtk_menu_item_new_with_label(*strings++);
        gtk_menu_shell_append((GtkMenuShell *)menubar, menu_item);
        GtkWidget *submenus= gtk_menu_new();
        gtk_menu_item_set_submenu((GtkMenuItem *)menu_item, submenus);
        for (j=0; j< sub_menu_num[i]; j++, k++)
        {
            GtkWidget *submenu= gtk_menu_item_new_with_label(*strings++);
            g_signal_connect(submenu, "activate", G_CALLBACK(*(handle+k)), (gpointer)(win));
            gtk_menu_shell_append(GTK_MENU_SHELL(submenus), submenu);
        }
    }
}

char *choose_file(int type)
{
    GtkWidget *filechooser;
    if (type==OPEN_FILE)
    {
        filechooser=gtk_file_chooser_dialog_new("打开", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
    }
    else if (type== SAVE_FILE)
    {
        filechooser=gtk_file_chooser_dialog_new("新建", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filechooser), TRUE);
    }
    else if (type== SAVE_FILE_AS)
    {
        filechooser=gtk_file_chooser_dialog_new("另存为", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, ("取消"), GTK_RESPONSE_CANCEL, ("确定"), GTK_RESPONSE_ACCEPT, NULL);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filechooser), TRUE);
    }
    GtkFileFilter *filter= gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.dord");
    gtk_file_filter_set_name(filter, "Dormitory Data Files(*.dord)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), filter);
    if (gtk_dialog_run (GTK_DIALOG (filechooser)) == GTK_RESPONSE_ACCEPT)
    {
        char *tfilename, *filename= malloc(300);
        tfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (filechooser));
        strcpy(filename, tfilename);
        g_free(tfilename);
        if ((type==SAVE_FILE) || (type== SAVE_FILE_AS))
        {
            int len= strlen(filename);
            if ((filename[len-5]!='.') || (filename[len-4]!='d') || (filename[len-3]!='o') || (filename[len-2]!='r') || (filename[len-1]!='d'))
                strcat(filename, ".dord");
        }
        gtk_widget_destroy(filechooser);
        return filename;
    }
    else
    {
        gtk_widget_destroy(filechooser);
        return NULL;
    }

}



void type_dialogue(int is_edit)
{
    GtkWidget *new_type_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_type_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_type_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_type_win), TRUE);

    gtk_window_set_position (GTK_WINDOW (new_type_win), GTK_WIN_POS_CENTER);
    if (is_edit)
    {
        gtk_window_set_title (GTK_WINDOW (new_type_win), "修改房间类型");
    }
    else
    {
        gtk_window_set_title (GTK_WINDOW (new_type_win), "添加新房间类型");
    }

    GtkWidget *box1= gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(new_type_win), box1);

    GtkWidget *box2= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label1= gtk_label_new("    房间类型：");
    GtkWidget *entry1= gtk_entry_new();
    GtkWidget *confirmbutton= gtk_button_new_with_label("确定");
    gtk_box_pack_start(GTK_BOX(box2), label1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), entry1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box2), confirmbutton, FALSE, TRUE,0);

    GtkWidget *box3= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    GtkWidget *label2= gtk_label_new("最多容纳人数：");
    GtkWidget *entry2= gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry2), GTK_INPUT_PURPOSE_DIGITS);
    GtkWidget *cancelbutton= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box3), label2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), entry2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box3), cancelbutton, FALSE, TRUE,0);

    gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box3, TRUE, TRUE, 0);

    if (is_edit)
    {
        gtk_entry_set_text(GTK_ENTRY(entry1), active_type->type_);
        char most_in[10];
        sprintf(most_in, "%d", active_type->most_in_);
        gtk_entry_set_text(GTK_ENTRY(entry2), most_in);
    }

    struct new_type_data *data=(struct new_type_data *)malloc(sizeof(struct new_type_data));
    data->entry1= entry1;
    data->entry2= entry2;
    if (is_edit)
        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_edit_type), (gpointer) data);
    else
        g_signal_connect(confirmbutton, "clicked", G_CALLBACK(handle_new_type), (gpointer) data);
    g_signal_connect_after(confirmbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_type_win);
    g_signal_connect(cancelbutton, "clicked", G_CALLBACK(handle_destroy_dialogue), (gpointer) new_type_win);
    gtk_widget_show_all (new_type_win);

};

void room_dialogue(int is_edit, struct room_dialogue_check *checkdata)
{
    GtkWidget *new_room_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_room_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_room_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_room_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_room_win), GTK_WIN_POS_CENTER);

    if (is_edit)
    {
        gtk_window_set_title(GTK_WINDOW(new_room_win), "修改房间信息");
    }
    else
    {
        gtk_window_set_title(GTK_WINDOW(new_room_win), "添加新房间");
    }

    GtkWidget *grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(new_room_win), grid);

    GtkWidget *box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    GtkWidget *label1= gtk_label_new("房间类型");
    GtkWidget *combo1= gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo1), TYPE_COLUMN);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1), 0);
    GtkCellRenderer *render= gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo1), render, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo1), render, "text", TYPE_COLUMN, NULL);

    GtkWidget *label2= gtk_label_new("房间号");
    GtkWidget *entry1= gtk_entry_new();

    GtkWidget *label3= gtk_label_new("电话号码");
    GtkWidget *entry2= gtk_entry_new();

    GtkWidget *label4= gtk_label_new("面积");
    GtkWidget *entry3= gtk_entry_new();

    GtkWidget *label5= gtk_label_new("价格");
    GtkWidget *entry4= gtk_entry_new();

    GtkWidget *checkbutton= gtk_check_button_new_with_label("空闲");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);

    if (is_edit)
    {
        gtk_entry_set_text(GTK_ENTRY(entry1), active_room->room_number_);
        gtk_entry_set_text(GTK_ENTRY(entry2), active_room->phone_number_);
        char area[20];
        char price[20];
        sprintf(area, "%f", active_room->area_);
        sprintf(price, "%f", active_room->price_);
        gtk_entry_set_text(GTK_ENTRY(entry3), area);
        gtk_entry_set_text(GTK_ENTRY(entry4), price);
    }

    GtkWidget *button1= gtk_button_new_with_label("确定");
    GtkWidget *button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0 ,3 ,1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbutton, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 5, 1, 1);

    struct new_room_data *data=(struct new_room_data *)malloc(sizeof(struct new_room_data));
    data->combo1= combo1;
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->checkbutton= checkbutton;

    if (is_edit)
    {
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_room_modify), data);
    }
    else
    {
        g_signal_connect(button1, "clicked", G_CALLBACK(handle_room_new), data);
    }
    if (checkdata)
        g_signal_connect(button1, "clicked", G_CALLBACK(room_dialogue_check), checkdata);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_room_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_room_win);

    gtk_widget_show_all (new_room_win);
}

void guest_in_dialogue(struct guest_dialogue_check *checkdata)
{
    GtkWidget *new_guest_in_win;
    GtkWidget *grid;
    GtkWidget *box1;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *button1;
    GtkWidget *button2;

    new_guest_in_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_guest_in_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_guest_in_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_guest_in_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_guest_in_win), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(new_guest_in_win), "添加新入住信息");

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(new_guest_in_win), grid);

    box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    label1= gtk_label_new("房间号");
    entry1= gtk_entry_new();

    label2= gtk_label_new("姓名");
    entry2= gtk_entry_new();

    label3= gtk_label_new("身份证号");
    entry3= gtk_entry_new();

    label4= gtk_label_new("入住日期");
    entry4= gtk_entry_new();
    time_t now;
    char stime[30];
    time(&now);
    strftime(stime, 30, "%Y/%m/%d-%H:%M", localtime(&now));
    gtk_entry_set_text(GTK_ENTRY(entry4), stime);

    button1= gtk_button_new_with_label("确定");
    button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 4, 1, 1);

    struct new_guest_in_data *data=(struct new_guest_in_data *)malloc(sizeof(struct new_guest_in_data));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_guest_in_new), data);
    if (checkdata)
        g_signal_connect(button1, "clicked", G_CALLBACK(guest_dialogue_check), checkdata);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_in_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_in_win);

    gtk_widget_show_all(new_guest_in_win);
}

void guest_out_dialogue()
{
    GtkWidget *new_guest_out_win;
    GtkWidget *grid;
    GtkWidget *box1;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *button1;
    GtkWidget *button2;

    new_guest_out_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_guest_out_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_guest_out_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_guest_out_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_guest_out_win), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(new_guest_out_win), "添加新退房信息");

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(new_guest_out_win), grid);

    box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    label1= gtk_label_new("房间号");
    entry1= gtk_entry_new();

    label2= gtk_label_new("身份证号");
    entry2= gtk_entry_new();

    label3= gtk_label_new("退房日期");
    entry3= gtk_entry_new();
    time_t now;
    char stime[30];
    time(&now);
    strftime(stime, 30, "%Y/%m/%d-%H:%M", localtime(&now));
    gtk_entry_set_text(GTK_ENTRY(entry3), stime);

    label4= gtk_label_new("付款");
    entry4= gtk_entry_new();

    button1= gtk_button_new_with_label("确定");
    button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 4, 1, 1);

    struct new_guest_out_data *data=(struct new_guest_out_data *)malloc(sizeof(struct new_guest_out_data));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_guest_out_new), data);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_out_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_out_win);

    gtk_widget_show_all(new_guest_out_win);
}

void guest_modify_dialogue(struct guest_dialogue_check *checkdata)
{
    GtkWidget *new_guest_win;
    GtkWidget *grid;
    GtkWidget *box1;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *entry4;
    GtkWidget *entry5;
    GtkWidget *entry6;
    GtkWidget *button1;
    GtkWidget *button2;

    new_guest_win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(new_guest_win), GTK_WINDOW(win));
    gtk_window_set_modal(GTK_WINDOW(new_guest_win), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(new_guest_win), TRUE);
    gtk_window_set_position (GTK_WINDOW (new_guest_win), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(new_guest_win), "编辑入住信息");

    grid= gtk_grid_new();
    gtk_grid_insert_column(GTK_GRID(grid), 0);
    gtk_grid_insert_column(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 0);
    gtk_grid_insert_row(GTK_GRID(grid), 1);
    gtk_grid_insert_row(GTK_GRID(grid), 2);
    gtk_grid_insert_row(GTK_GRID(grid), 3);
    gtk_grid_insert_row(GTK_GRID(grid), 4);
    gtk_grid_insert_row(GTK_GRID(grid), 5);
    gtk_grid_insert_row(GTK_GRID(grid), 6);
    gtk_container_add(GTK_CONTAINER(new_guest_win), grid);

    box1= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(box1), TRUE);

    label1= gtk_label_new("房间号");
    entry1= gtk_entry_new();

    label2= gtk_label_new("姓名");
    entry2= gtk_entry_new();

    label3= gtk_label_new("身份证号");
    entry3= gtk_entry_new();

    label4= gtk_label_new("入住日期");
    entry4= gtk_entry_new();

    label5= gtk_label_new("退房日期");
    entry5= gtk_entry_new();

    label6= gtk_label_new("付款");
    entry6= gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(entry1), active_guest->room_info_->room_number_);
    gtk_entry_set_text(GTK_ENTRY(entry2), active_guest->name_);
    gtk_entry_set_text(GTK_ENTRY(entry3), active_guest->id_);
    gtk_entry_set_text(GTK_ENTRY(entry4), active_guest->in_time_);
    gtk_entry_set_text(GTK_ENTRY(entry5), active_guest->out_time_);
    char pay[20];
    sprintf(pay, "%f", active_guest->pay_);
    gtk_entry_set_text(GTK_ENTRY(entry6), pay);

    button1= gtk_button_new_with_label("确定");
    button2= gtk_button_new_with_label("取消");
    gtk_box_pack_start(GTK_BOX(box1), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box1), button2, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry3, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry4, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry5, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label6, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry6, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), box1, 1, 6, 1, 1);

    struct new_guest_data *data=(struct new_guest_data *)malloc(sizeof(struct new_guest_data));
    data->entry1= entry1;
    data->entry2= entry2;
    data->entry3= entry3;
    data->entry4= entry4;
    data->entry5= entry5;
    data->entry6= entry6;

    g_signal_connect(button1, "clicked", G_CALLBACK(handle_guest_modify), data);
    if (checkdata)
        g_signal_connect(button1, "clicked", G_CALLBACK(guest_dialogue_check), checkdata);
    g_signal_connect(button1, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_win);
    g_signal_connect(button2, "clicked", G_CALLBACK(handle_destroy_dialogue), new_guest_win);

    gtk_widget_show_all(new_guest_win);
}


void type_view_details()
{
    gtk_list_store_clear(roomstore);
    access_room_data();
    char title[25]= "客房类型详情:";
    strcat(title, active_type->type_);
    gtk_window_set_title (GTK_WINDOW (room_win), title);
    gtk_widget_show_all(room_win);

}

void room_view_detail()
{
    gtk_list_store_clear(gueststore);
    access_guest_data();
    char title[25]= "客房入住详情:";
    strcat(title, active_room->room_number_);
    gtk_window_set_title (GTK_WINDOW (guest_win), title);
    gtk_widget_show_all(guest_win);
}



/****************************************************
maintain data
****************************************************/
struct basic_guest_info *load_guest_data(struct basic_room_data *room)
{
    if (!feof(file))
    {
        struct basic_guest_info *loc_head, *loc_tail;
        loc_head= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
        fread(loc_head, sizeof(struct basic_guest_info), 1, file);
        loc_tail= loc_head;
        loc_tail->room_info_= room;
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
            fread(loc_tail->next_, sizeof(struct basic_guest_info), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->room_info_= room;
        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        return NULL;
    }
};

struct basic_room_data *load_room_data(struct basic_room_type_data *type, int *total, int *left)
{
    if (!feof(file))
    {
        struct basic_room_data *loc_head, *loc_tail;
        loc_head= (struct basic_room_data *)malloc(sizeof(struct basic_room_data));
        fread(loc_head, sizeof(struct basic_room_data), 1, file);
        loc_tail= loc_head;
        loc_tail->type_=type;
        (*total)++;
        if (loc_tail->is_left_)
            (*left)++;
        if (loc_tail->guest_)
        {
            loc_tail->guest_= load_guest_data(loc_tail);
            if (!loc_tail->guest_)
                return NULL;
        }
        while ((loc_tail->next_) && !feof(file))
        {
            loc_tail->next_= (struct basic_room_data *)malloc(sizeof(struct basic_room_data));
            fread(loc_tail->next_, sizeof(struct basic_room_data), 1, file);
            loc_tail= loc_tail->next_;
            loc_tail->type_=type;
            (*total)++;
            if (loc_tail->is_left_)
                (*left)++;
            if (loc_tail->guest_)
            {
                loc_tail->guest_= load_guest_data(loc_tail);
                if (!loc_tail->guest_)
                    return NULL;
            }

        }
        if (loc_tail->next_)
        {
            return NULL;
        }
        else
            return loc_head;
    }
    else
    {
        errmesg("File Damaged.");
        return NULL;
    }
};

struct basic_room_type_data *load_data()
{
    if (file)
    {

        if (!feof(file))
        {
            struct basic_room_type_data *loc_head, *loc_tail;
            loc_head= (struct basic_room_type_data *)malloc(sizeof(struct basic_room_type_data));
            memset(loc_head, 0, sizeof(struct basic_room_type_data));
            if (!fread(loc_head, sizeof(struct basic_room_type_data), 1, file))
            {
                free(loc_head);
                return NULL;
            }
            loc_tail= loc_head;
            loc_tail->total_= 0;
            loc_tail->left_=0;
            if (loc_tail->room_head_)
            {
                loc_tail->room_head_= load_room_data(loc_tail, &loc_tail->total_, &loc_tail->left_);
                if (!loc_tail->room_head_)
                    return NULL;
            }
            while ((loc_tail->next_) && !feof(file))
            {
                loc_tail->next_= (struct basic_room_type_data *)malloc(sizeof(struct basic_room_type_data));
                fread(loc_tail->next_, sizeof(struct basic_room_type_data), 1, file);
                loc_tail= loc_tail->next_;
                loc_tail->total_= 0;
                loc_tail->left_=0;
                if (loc_tail->room_head_)
                {
                    loc_tail->room_head_= load_room_data(loc_tail, &loc_tail->total_, &loc_tail->left_);
                    if (!loc_tail->room_head_)
                        return NULL;
                }

            }
            if (loc_tail->next_)
                return NULL;
            else
                return loc_head;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void save_data()
{
    if (!file)
    {
        errmesg("No File Opened.");
    }
    else
    {
        rewind(file);
        struct basic_room_type_data *type_itor= type_head;
        while (type_itor)
        {
            fwrite(type_itor, sizeof(struct basic_room_type_data), 1, file);
            struct basic_room_data *room_itor= type_itor->room_head_;
            while (room_itor)
            {
                fwrite(room_itor, sizeof(struct basic_room_data), 1, file);
                struct basic_guest_info *guest_itor= room_itor->guest_;
                while (guest_itor)
                {
                    fwrite(guest_itor, sizeof(struct basic_guest_info), 1, file);
                    guest_itor= guest_itor->next_;
                }
                room_itor= room_itor->next_;
            }
            type_itor= type_itor->next_;
        }
        edited= 0;
    }
}

void type_insert(const char *type, int most_in)
{
    struct basic_room_type_data *tmp= (struct basic_room_type_data *)malloc(sizeof(struct basic_room_type_data));
    strcpy(tmp->type_, type);
    tmp->most_in_= most_in;
    tmp->total_= 0;
    tmp->left_= 0;
    tmp->room_head_=NULL;
    tmp->next_= type_head;
    type_head= tmp;
}


void type_modify(const char *type, int most_in)
{
    strcpy(active_type->type_, type);
    active_type->most_in_= most_in;
}

void type_delete()
{
    struct basic_room_type_data *iter= type_head;
    struct basic_room_data *room_iter= active_type->room_head_;
    while (room_iter)
    {
        struct basic_guest_info *guest_iter= room_iter->guest_;
        while (guest_iter)
        {
            struct basic_guest_info *tmp= guest_iter;
            guest_iter= guest_iter->next_;
            free(tmp);
        }
        struct basic_room_data *tmp= room_iter;
        room_iter= room_iter->next_;
        free(tmp);
    };
    if (iter== active_type)
    {
        type_head= type_head->next_;
        free(iter);
    }
    else
    {
        for (; iter->next_!= active_type; iter= iter->next_)
            ;
        iter->next_= active_type->next_;
        free(active_type);
    }

}

void room_insert(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left)
{
    struct basic_room_data *tmp=malloc(sizeof(struct basic_room_data));
    tmp->type_= type;
    strcpy(tmp->room_number_, room_number);
    strcpy(tmp->phone_number_, phone_number);
    tmp->area_= area;
    tmp->price_= price;
    tmp->is_left_= is_left;
    tmp->guest_= NULL;
    tmp->next_= type->room_head_;
    type->room_head_= tmp;
    type->total_++;
    if (is_left)
        type->left_++;

}

void room_modify(struct basic_room_type_data *type, const char *room_number, const char *phone_number, float area, float price, int is_left)
{
    if (type== active_type)
    {
        active_room->area_= area;
        strcpy(active_room->phone_number_, phone_number);
        strcpy(active_room->room_number_, room_number);
        active_room->price_= price;
        if (active_room->is_left_> is_left)
            active_type->left_--;
        else if (active_room->is_left_ < is_left)
            active_type->left_++;
        active_room->is_left_= is_left;
    }
    else
    {
        room_delete();
        room_insert(type, room_number, phone_number, area, price, is_left);
    }
}

void room_delete()
{
    active_type->total_--;
    if (active_room->is_left_)
        active_type->left_--;
    struct basic_guest_info *guest_iter= active_room->guest_;
    while (guest_iter)
    {
        struct basic_guest_info *tmp= guest_iter;
        guest_iter= guest_iter->next_;
        free(tmp);
    }
    if (active_type->room_head_== active_room)
    {
        active_type->room_head_= active_room->next_;
        free(active_room);
    }
    else
    {
        struct basic_room_data *room_iter= active_type->room_head_;
        while (room_iter->next_!= active_room)
        {
            room_iter= room_iter->next_;
        }
        room_iter->next_= active_room->next_;
        free(active_room);
    }

}

void guest_in_insert(struct basic_room_data *room, const char *id, const char *name, const char *in_time)
{
    room->is_left_= 0;
    room->type_->left_--;
    struct basic_guest_info *tmp= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
    tmp->room_info_= room;
    strcpy(tmp->id_, id);
    strcpy(tmp->name_, name);
    strcpy(tmp->in_time_, in_time);
    tmp->day_spent_= 0;
    tmp->fare_= 0;
    tmp->out_time_[0]= 0;
    tmp->pay_= 0;
    tmp->next_= room->guest_;
    room->guest_= tmp;
}

void guest_out_insert(struct basic_guest_info *guest, const char *out_time, float pay)
{
    guest->room_info_->is_left_= 1;
    guest->room_info_->type_->left_++;
    strcpy(guest->out_time_, out_time);
    guest->day_spent_= cal_day(guest->in_time_, guest->out_time_);
    guest->fare_= guest->room_info_->price_*guest->day_spent_;
    guest->pay_= pay;
}

void guest_modify(struct basic_room_data *room, const char *id, const char *name, const char *in_time, const char *out_time, float pay)
{
    if (room== active_room)
    {
        strcpy(active_guest->id_, id);
        strcpy(active_guest->name_, name);
        strcpy(active_guest->in_time_, in_time);
        active_guest->pay_= pay;
        if (out_time[0]== 0)
        {
            if (active_guest->out_time_[0]!=0)
            {
                active_room->is_left_= 0;
                active_type->left_--;
            }
            strcpy(active_guest->out_time_, out_time);
            active_guest->day_spent_= 0;
            active_guest->fare_= 0;
        }
        else
        {
            if (active_guest->out_time_[0]== 0)
            {
                active_room->is_left_= 1;
                active_type->left_++;
            }
            strcpy(active_guest->out_time_, out_time);
            active_guest->day_spent_= cal_day(active_guest->in_time_, active_guest->out_time_);
            active_guest->fare_= active_room->price_*active_guest->day_spent_;
        }
    }
    else
    {
        guest_delete();
        guest_in_insert(room, id, name, in_time);
        guest_out_insert(room->guest_, out_time, pay);
    }
}

void guest_delete()
{
    if (active_guest->out_time_[0]== 0)
    {
        active_guest->room_info_->is_left_= 1;
        active_guest->room_info_->type_->left_++;
    }

    if (active_room->guest_ == active_guest)
    {
        active_room->guest_= active_guest->next_;
        free(active_guest);
    }
    else
    {
        struct basic_guest_info *guest_iter= active_guest->room_info_->guest_;
        while (guest_iter->next_!= active_guest)
            guest_iter=guest_iter->next_;
        guest_iter->next_= active_guest->next_;
        free(active_guest);
    }

}
/****************************************************
query
****************************************************/
void q_room_by_conditions(GtkListStore *store,  int is_room_number_limited, int is_phone_number_limited, int is_type_limited, int is_left_limited, int is_price_limited, int is_area_limited, struct basic_room_type_data *type, const char *room_number, const char *phone_number, int is_left, float uprice, float lprice, float uarea, float larea)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    GtkTreeIter iter;
    gtk_list_store_clear(store);
    while (type_iter)
    {
        if (!is_type_limited || (type_iter== type))
        {
            room_iter= type_iter->room_head_;
            while (room_iter)
            {
                if (((!is_room_number_limited)||!strcmp(room_number, room_iter->room_number_)) && ((!is_phone_number_limited)||!strcmp(phone_number, room_iter->phone_number_)) && ((!is_left_limited)||(is_left== room_iter->is_left_)) && ((!is_price_limited)||((room_iter->price_ <=uprice) && (room_iter->price_>= lprice))) && ((!is_area_limited)||((room_iter->area_<= uarea) && (room_iter->area_>= larea))))
                    gtk_list_store_insert_with_values(store, &iter, -1,
                                                      ROOM_POINTER_COLUMN, room_iter,
                                                      ROOM_NUMBER_COLUMN, room_iter->room_number_,
                                                      PHONE_NUMBER_COLUMN, room_iter->phone_number_,
                                                      AREA_COLUMN, room_iter->area_,
                                                      PRICE_COLUMN,room_iter->price_,
                                                      IS_LIFT_COLUMN, room_iter->is_left_?"是": "否",
                                                      ROOM_COLUMN_NUMBER, room_iter->type_->type_,
                                                      -1);
                room_iter= room_iter->next_;
            }
        }
        type_iter= type_iter->next_;
    }
}
void q_guest_info_by_conditions(GtkListStore *store, int is_id_limited, int is_name_limited, int is_in_time_limited, const char *id, const char *name_piece, const char *lin_time, const char *uin_time)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    struct basic_guest_info *guest_iter;
    GtkTreeIter iter;
    gtk_list_store_clear(store);
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            guest_iter= room_iter->guest_;
            while (guest_iter)
            {
                if (((!is_id_limited)||(!strcmp(id, guest_iter->id_))) && ((!is_name_limited) || (strstr(guest_iter->name_, name_piece))) && ((!is_in_time_limited) || ((cal_day(lin_time, guest_iter->in_time_)>=0) && (cal_day(uin_time, guest_iter->in_time_) <=0))))
                    gtk_list_store_insert_with_values(store, &iter, -1,
                                                      GUEST_POINTER_COLUMN, guest_iter,
                                                      GUEST_ID_COLUMN, guest_iter->id_,
                                                      GUEST_NAME_COLUMN, guest_iter->name_,
                                                      GUEST_IN_TIME_COLUMN, guest_iter->in_time_,
                                                      GUEST_OUT_TIME_COLUMN, (guest_iter->out_time_[0]==0)?"在住":guest_iter->out_time_,
                                                      GUEST_DAY_SPENT_COLUMN, guest_iter->day_spent_,
                                                      GUEST_FARE_COLUMN, guest_iter->fare_,
                                                      GUEST_PAID_COLUMN, guest_iter->pay_,
                                                      GUEST_COLUMN_NUMBER, guest_iter->room_info_->room_number_,
                                                      -1);
                guest_iter= guest_iter->next_;
            }
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
}
/*****************************************************
Assistant
*****************************************************/
int get_hour(const char timech[])
{
    const char * iter= timech;
    while (*iter++!= '-')
        ;
    int hour= 0;
    while (*iter!= ':')
    {
        hour*= 10;
        hour+= *iter- '0';
        iter++;
    }
    return hour;
}

int is_year(int year)
{
    return (!(year % 4) && (year % 100))|| !(year % 400);
}

int day_of_month(int year, int month)
{
    static const int daymonth[13]= {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((month==2) && is_year(year))
        return daymonth[month]+1;
    else
        return  daymonth[month];
}

int day_of_year(int year)
{
    return is_year(year)?366:365;
}

int date_2_day(int year, int month, int day)
{
    int result= 0;
    result+= day;
    result+= day_since[month];
    result+=is_year(year);
    return result;
}

int get_num(const char **chs, char ch)
{
    int result= 0;
    while (**chs!= ch)
    {
        result*= 10;
        result+= **chs - '0';
        (*chs)++;
    }
    (*chs)++;
    return result;
}

float cal_day(const char time1[], const char time2[])
{
    int year1= 0, year2= 0, month1= 0, month2= 0, day1= 0, day2= 0;
    const char *ch1= time1, *ch2= time2;

    year1= get_num(&ch1, '/');
    month1= get_num(&ch1, '/');
    day1= get_num(&ch1, '-');
    if (*ch2 == 0)
    {
        time_t now;
        time(&now);
        char stime[50];
        strftime(stime, 50, "%Y/%m/%d-%H:%M", localtime(&now));
        ch2= stime;
        year2= get_num(&ch2, '/');
        month2= get_num(&ch2, '/');
        day2= get_num(&ch2, '-');
    }
    else
    {
        year2= get_num(&ch2, '/');
        month2= get_num(&ch2, '/');
        day2= get_num(&ch2, '-');
    }

    float result=0;
    if (year1!= year2)
    {
        int i;
        result+= day_of_year(year1)- date_2_day(year1, month1, day1);
        for (i= year1+1; i< year2; i++)
            result+= day_of_year(i);
        result+=date_2_day(year2, month2, day2);

    }
    else
    {
        result= date_2_day(year2, month2, day2)- date_2_day(year1, month1, day1);
    }

    if (get_hour(time2) >= 18)
        result++;
    else if (get_hour(time2) >= 12)
        result+= 0.5;

    return result;
}

int cal_types()
{
    int result= 0;
    struct basic_room_type_data *type_iter= type_head;
    while (type_iter)
    {
        result++;
        type_iter= type_iter->next_;
    }
    return result;
}

void fill_type(int types, GType *type_list)
{
    int i;
    *type_list= G_TYPE_STRING;
    for (i=1; i<= types; i++)
        type_list[i]= G_TYPE_FLOAT;
}

float cal_guest(struct basic_guest_info *guest_iter, int year, int month)
{
    const char *in_time= guest_iter->in_time_, *out_time= guest_iter->out_time_;
    if (out_time[0]== 0)
    {
        guest_iter= guest_iter->next_;
        return 0;
    }
    int in_year= get_num(&in_time, '/'), in_month= get_num(&in_time, '/'), in_day= get_num(&in_time, '-');
    int out_year= get_num(&out_time, '/'), out_month= get_num(&out_time, '/'), out_day= get_num(&out_time, '-');
    float day= 0;
    if (((in_year> year) || ((in_year== year) && (in_month> month)))||((out_year< year) || ((out_year== year) && (out_month< month))))
    {
        guest_iter= guest_iter->next_;
        return 0;
    }
    int out_hour= get_num(&out_time, ':');
    if ((in_year< year) || ((in_year== year) && (in_month< month)))
    {
        in_year= year;
        in_month= month;
        in_day= 1;
    }
    else
        day-=1;
    if ((out_year> year) || ((out_year== year) && (out_month> month)))
    {
        out_year= year;
        out_month= month;
        out_day= day_of_month(year, month);
        out_hour=0;
    }
    day+= out_day-in_day+1;
    if (out_hour>=12)
        day+=0.5;
    if (out_hour>= 18)
        day+=0.5;
    return day;
}

float cal_room(struct basic_room_data *room_iter, int year, int month)
{
    struct basic_guest_info *guest_iter= room_iter->guest_;
    float result= 0;
    while (guest_iter)
    {
        result+= guest_iter->pay_* cal_guest(guest_iter, year, month)/ cal_day(guest_iter->in_time_, guest_iter->out_time_);
        guest_iter= guest_iter->next_;
    }
    return result;
}

float cal_value(struct basic_room_type_data *type, int year, int month)
{
    struct basic_room_data *room_iter= type->room_head_;
    float result= 0;
    while (room_iter)
    {
        result+= cal_room(room_iter, year, month);
        room_iter= room_iter->next_;
    }
    return result;
}

void cal_values(int types, int year, int month, int *column_num, GValue *value, float *total)
{
    g_value_init(value, G_TYPE_STRING);
    char smonth[3];
    sprintf(smonth, "%d", month);
    g_value_set_string(value, smonth);
    column_num[0]=0;
    struct basic_room_type_data *type_iter= type_head;
    int i;
    for (i=1; i<= types; i++, type_iter= type_iter->next_)
    {
        column_num[i]= i;
        g_value_init(value+i, G_TYPE_FLOAT);
        float v= cal_value(type_iter, year, month);
        total[i]+= v;
        g_value_set_float(value+i, v);
    }
}

void cal_room_sta(GtkListStore *store, struct basic_room_data *room, int year, float *total_w, float *total_day)
{
    struct basic_guest_info *guest_iter= room->guest_;
    float w= 0, in_time= 0, in_time_real= 0;
    while (guest_iter)
    {
        int i;
        for (i=1; i<=12; i++)
        {
            float stime= cal_guest(guest_iter, year, i);
            in_time+= stime;
            w+= guest_iter->pay_ * stime / cal_day(guest_iter->in_time_, guest_iter->out_time_);
        }
        guest_iter= guest_iter->next_;
    }
    in_time_real= in_time / day_of_year(year);
    GtkTreeIter iter;
    gtk_list_store_insert_with_values(store, &iter, -1,
                                      STA_ROOM_NUMBER_COLUMN, room->room_number_,
                                      STA_ROOM_TYPE_COLUMN, room->type_,
                                      STA_W_COLUMN, w,
                                      STA_IN_TIME_COLUME, in_time,
                                      STA_IN_REAL_CULUME, in_time_real,
                                      -1);
    *total_w+= w;
    *total_day+= in_time;
}

void cal_rooms_sta(GtkListStore *store, int year)
{
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    float total_w= 0, total_day= 0;
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            cal_room_sta(store, room_iter, year, &total_w, &total_day);
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    GtkTreeIter iter;
    gtk_list_store_insert_with_values(store, &iter, -1,
                                      STA_ROOM_NUMBER_COLUMN, "总计",
                                      STA_W_COLUMN, total_w,
                                      STA_IN_TIME_COLUME, total_day,
                                      STA_IN_REAL_CULUME, total_day/day_of_year(year),
                                      -1);
}

struct basic_guest_info *ssort(struct basic_guest_info *guest_list)
{
    if ((!guest_list) || (!guest_list->next_))
        return guest_list;
    struct basic_guest_info *tmph= (struct basic_guest_info*) malloc(sizeof(struct basic_guest_info)), *max, *itor, *now1, *pre1;
    tmph->next_= guest_list;
    for (pre1=tmph, now1= guest_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->pay_> max->next_->pay_)
                max= itor;
        if (now1==max)
        {
            struct basic_guest_info *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct basic_guest_info *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct basic_guest_info *result= tmph->next_;
    free(tmph);
    return result;
}

void find_insert(struct basic_guest_info *guest, struct basic_guest_info **list_head)
{
    struct basic_guest_info *list= *list_head;
    while (list)
    {
        if (!strcmp(guest->id_, list->id_))
        {
            list->day_spent_+= guest->day_spent_;
            list->fare_+= guest->fare_;
            list->pay_+= guest->pay_;
            break;
        }
        list= list->next_;
    }
    if (!list)
    {
        struct basic_guest_info *tmp= (struct basic_guest_info *)malloc(sizeof(struct basic_guest_info));
        *tmp= *guest;
        tmp->next_= *list_head;
        *list_head= tmp;
    }
}

void cal_top_10_guest(GtkListStore *store)
{
    struct basic_guest_info *result_list= NULL;
    struct basic_room_type_data *type_iter= type_head;
    struct basic_room_data *room_iter;
    struct basic_guest_info *guest_iter;

    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            guest_iter= room_iter->guest_;
            while (guest_iter)
            {
                find_insert(guest_iter, &result_list);
                guest_iter= guest_iter->next_;
            }
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }
    result_list= ssort(result_list);
    int i=0;
    GtkTreeIter iter;
    while ((i<10) && (result_list))
    {
        gtk_list_store_insert_with_values(store, &iter, -1,
                                          STA_GUEST_RANK_COLUMN, i+1,
                                          STA_GUEST_ID_COLUMN, result_list->id_,
                                          STA_GUEST_NAME_COLUMN, result_list->name_,
                                          STA_GUEST_IN_TIME_COLUMN, result_list->day_spent_,
                                          STA_GUEST_FARE_COLUMN, result_list->fare_,
                                          STA_GUEST_PAID_COLUMN, result_list->pay_,
                                          STA_DISCOUNT_COLUMN, result_list->pay_/ result_list->fare_,
                                          -1);
        struct basic_guest_info *tmp =result_list;
        result_list= result_list->next_;
        free(tmp);
        i++;
    }
}

void cal_room_full(struct basic_room_data *room, struct room_interest *interest)
{
    struct basic_guest_info *guest_iter= room->guest_;
    interest->fare_= 0;
    interest->paid_ =0;
    strcpy(interest->room_number_, room->room_number_);
    interest->room_type_= room->type_->type_;
    while (guest_iter)
    {
        interest->fare_+= guest_iter->fare_;
        interest->paid_+= guest_iter->pay_;
        guest_iter= guest_iter->next_;
    }
}

struct room_interest *ssort2(struct room_interest *room_list)
{
    if ((!room_list) || (!room_list->next_))
        return room_list;
    struct room_interest *tmph= (struct room_interest*) malloc(sizeof(struct room_interest)), *max, *itor, *now1, *pre1;
    tmph->next_= room_list;
    for (pre1=tmph, now1= room_list; now1->next_; pre1= pre1->next_, now1= pre1->next_)
    {
        max= pre1;
        for (itor= pre1; itor->next_; itor= itor->next_)
            if (itor->next_->paid_> max->next_->paid_)
                max= itor;
        if (now1==max)
        {
            struct room_interest *tmp= max->next_;
            now1->next_= max->next_->next_;
            tmp->next_= now1;
            pre1->next_= tmp;

        }
        else
        {
            struct room_interest *tmp= max->next_->next_;
            max->next_->next_= now1->next_;
            now1->next_= tmp;
            pre1->next_= max->next_;
            max->next_= now1;
        }
    }
    struct room_interest *result= tmph->next_;
    free(tmph);
    return result;
}


void cal_top_room(GtkListStore *store)
{
    struct room_interest *list_head= NULL;
    struct basic_room_type_data *type_iter;
    struct basic_room_data *room_iter;

    type_iter= type_head;
    while (type_iter)
    {
        room_iter= type_iter->room_head_;
        while (room_iter)
        {
            struct room_interest *tmp= (struct room_interest *)malloc(sizeof(struct room_interest));
            cal_room_full(room_iter, tmp);
            tmp->next_= list_head;
            list_head= tmp;
            room_iter= room_iter->next_;
        }
        type_iter= type_iter->next_;
    }

    list_head= ssort2(list_head);

    int i= 1;
    while (list_head)
    {
        GtkTreeIter iter;
        gtk_list_store_insert_with_values(store, &iter, -1,
                                          STA_ROOM_RANK_COLUMN, i,
                                          STA_ROOM_NUMBER_COLUMN2, list_head->room_number_,
                                          STA_ROOM_TYPE_COLUMN2, list_head->room_type_,
                                          STA_ROOM_FARE_COLUMN, list_head->fare_,
                                          STA_ROOM_PAID_COLUMN, list_head->paid_,
                                          STA_ROOM_DISCOUNT_COLUMN, list_head->fare_?list_head->paid_/list_head->fare_:0,
                                          -1);
        struct room_interest *tmp= list_head;
        list_head= list_head->next_;
        free(tmp);
        i++;
    }
}
