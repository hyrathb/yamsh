#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#define MENU_NUM 5

const int sub_menu_num[MENU_NUM]= {5, 4, 2, 3, 2};

const int day_since[13]={0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

const char * const menu_string[]=
{
    "文件", "新建", "打开", "保存", "关闭", "退出",
    "编辑", "新类型", "新客房", "新订房记录", "新退房记录",
    "查询", "客房信息查询", "旅客信息查询",
    "统计", "按月统计", "按年统计", "按旅客统计",
    "帮助", "关于", "帮助"
};


enum
{
    OPEN_FILE,
    SAVE_FILE
};

enum
{
    POINTER_COLUMN,
    TYPE_COLUMN,
    MOST_IN_COLUMN,
    TOTAL_COLUMN,
    LEFT_COLUMN,
    COLUMN_NUMBER
};

enum
{
    ROOM_POINTER_COLUMN,
    ROOM_NUMBER_COLUMN,
    PHONE_NUMBER_COLUMN,
    AREA_COLUMN,
    PRICE_COLUMN,
    IS_LIFT_COLUMN,
    ROOM_COLUMN_NUMBER
};

enum
{
    GUEST_POINTER_COLUMN,
    GUEST_ID_COLUMN,
    GUEST_NAME_COLUMN,
    GUEST_IN_TIME_COLUMN,
    GUEST_OUT_TIME_COLUMN,
    GUEST_DAY_SPENT_COLUMN,
    GUEST_FARE_COLUMN,
    GUEST_PAID_COLUMN,
    GUEST_COLUMN_NUMBER
};

enum
{
    STA_ROOM_NUMBER_COLUMN,
    STA_ROOM_TYPE_COLUMN,
    STA_W_COLUMN,
    STA_IN_TIME_COLUME,
    STA_IN_REAL_CULUME,
    STA_ROOM_COLUMN_NUMBER
};

enum
{
    STA_GUEST_RANK_COLUMN,
    STA_GUEST_ID_COLUMN,
    STA_GUEST_NAME_COLUMN,
    STA_GUEST_IN_TIME_COLUMN,
    STA_GUEST_FARE_COLUMN,
    STA_GUEST_PAID_COLUMN,
    STA_DISCOUNT_COLUMN,
    STA_GUEST_COLUMN_NUMBER

};

enum
{
    NEW,
    EDIT
};

FILE *file;

int edited= 0;

struct basic_room_type_data *type_head;

struct basic_room_type_data *active_type= NULL;

struct basic_room_data *active_room= NULL;

struct basic_guest_info *active_guest= NULL;

GtkWidget *win;

GtkWidget *room_win;

GtkWidget *guest_win;

GtkListStore *liststore;

GtkListStore *roomstore;

GtkListStore *gueststore;

GtkTreeIter treeiter;

#endif // DEFINES_H_INCLUDED
