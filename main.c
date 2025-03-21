#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#define WIDTH 1200
#define HEIGHT 800

typedef struct {
    int x, y, w, h;
} Rect;

void dessinerTout(Rect r) {
    al_clear_to_color(al_map_rgb(220, 220, 230));
    al_draw_filled_rectangle(r.x, r.y, r.x+r.w, r.y+r.h,al_map_rgb(0, 0, 255));
    al_flip_display();
}

void deplacerRectGauche(Rect* r) {
    r->x -= 20;
    if(r->x < 0) {
        r->x = 0;
    }
}

int main(void) {
    // Declaration
    Rect rectangle;
    ALLEGRO_DISPLAY*  fenetre ;
    ALLEGRO_EVENT_QUEUE* file = NULL;
    ALLEGRO_TIMER* timer =  NULL;
    ALLEGRO_COLOR noir;
    ALLEGRO_EVENT event;
    bool fini = false;
    bool clic = false;
    int deltax = 0;
    int deltay = 0;


    // initialisation
    assert(al_init());
    assert(al_init_primitives_addon());
    assert(al_install_keyboard());
    assert(al_install_mouse());

    rectangle.w = 100;
    rectangle.h = 60;
    rectangle.x = WIDTH / 2 - rectangle.w / 2;
    rectangle.y = HEIGHT / 2 - rectangle.h / 2;

    // creation
    fenetre = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(fenetre, "ECE");

    timer = al_create_timer(1.0/60.0); // frequence de 24 frames/s
    al_start_timer(timer);


    // sources
    file = al_create_event_queue();
    al_register_event_source(file, al_get_keyboard_event_source());
    al_register_event_source(file, al_get_display_event_source(fenetre));
    al_register_event_source(file, al_get_timer_event_source(timer));
    al_register_event_source(file, al_get_mouse_event_source());

    // premier affichage
    dessinerTout(rectangle);

    // boucle principale
    while(!fini) {
        al_wait_for_event(file, &event);
        switch(event.type) {
        case ALLEGRO_EVENT_KEY_DOWN: {
                switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT: {
                        deplacerRectGauche(&rectangle);
                        break;
                }
                }
                break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            fini = true;
            break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            if (clic)
            {
                rectangle.x = event.mouse.x - deltax;
                rectangle.y = event.mouse.y - deltay;
            }
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (rectangle.x < event.mouse.x && event.mouse.x < rectangle.x + rectangle.w &&
                rectangle.y < event.mouse.y && event.mouse.y < rectangle.y + rectangle.h )
            {
                clic = true;
                deltax = event.mouse.x - rectangle.x;
                deltay = event.mouse.y - rectangle.y;
            }
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            clic = false;
            break;
        case ALLEGRO_EVENT_TIMER:
            dessinerTout(rectangle);
            break;
        }

    }

    al_destroy_display(fenetre);
    al_destroy_event_queue(file);
    al_destroy_timer(timer);
    return 0;
}
