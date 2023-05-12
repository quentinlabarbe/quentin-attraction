#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct personnage
{
    BITMAP *bas[3];
    BITMAP *gauche[3];
    BITMAP *droite[3];
    BITMAP *haut[3];
    int x, y;
    int direction;
    int frame;
    int en_deplacement;
} t_personnage;



void initialisationAllegro();

void separer_bitmap_personnage_un(t_personnage *personnage, BITMAP *bitmap);

void dessiner_personnage(t_personnage *personnage, BITMAP *buffer);

void maj_personnage(t_personnage *personnage);

void maj_personnage_deux(t_personnage *personnage_deux);

int main()
{
    //declaration de variables
    BITMAP *imageChargee; //fond_carte
    BITMAP *buffer; //�cran
    BITMAP *sprite; //personnage
    BITMAP *sprite_deux;
    //initialisation allegro mi dans un sous-programme
    initialisationAllegro();


    install_keyboard();
    install_mouse();



    //chargement de l'image
    imageChargee = load_bitmap(
            "..\\images\\image_plan_carte.bmp", NULL);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    sprite = load_bitmap("..\\images\\sprite.bmp", NULL);
    sprite_deux = load_bitmap("..\\images\\sprite_deux.bmp",
                              NULL);

    if (!imageChargee)
    {
        allegro_message("Pb de l'image chargee");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    if (!sprite)
    {
        allegro_message("Erreur lors du chargement de 'sprite.bmp'\n");
        destroy_bitmap(imageChargee);
        destroy_bitmap(buffer);
        return 1;
    }
    if (!sprite_deux)
    {
        allegro_message("Erreur lors du chargement de 'sprite_deux.bmp'\n");
        destroy_bitmap(imageChargee);
        destroy_bitmap(buffer);
        return 1;
    }

    t_personnage personnage;
    personnage.direction = 3;
    personnage.frame = 0;
    personnage.x = 294;
    personnage.y = 580;

    t_personnage personnage_deux;
    personnage_deux.direction = 3;
    personnage_deux.frame = 0;
    personnage_deux.x = 285;
    personnage_deux.y = 580;

    separer_bitmap_personnage_un(&personnage, sprite);
    separer_bitmap_personnage_un(&personnage_deux, sprite_deux);

    int frame_counter = 0;

    // boucle �v�nements
    while (!key[KEY_ESC])
    {
        //affichage de l'image charg�e
        maj_personnage(&personnage);
        maj_personnage_deux(&personnage_deux);

        stretch_blit(imageChargee, buffer, 0, 0, imageChargee->w, imageChargee->h, 0, 0, buffer->w, buffer->h);

        dessiner_personnage(&personnage, buffer);
        dessiner_personnage(&personnage_deux, buffer);
        //blit(imageChargee, buffer, 0, 0, 0, 0, imageChargee->w, imageChargee->h);

        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);


        frame_counter++;
        if (frame_counter >= 10)
        {
            if (personnage.en_deplacement && personnage_deux.en_deplacement)
            {
                personnage.frame = (personnage.frame + 1) % 3;
                personnage_deux.frame = (personnage_deux.frame + 1) % 3;
            }
            else
            {
                personnage.frame = 0;
                personnage_deux.frame = 0;
            }
            frame_counter = 0;
        }

        rest(10);


    }

    for (int i = 0; i < 3; i++)
    {
        destroy_bitmap(personnage.bas[i]);
        destroy_bitmap(personnage.gauche[i]);
        destroy_bitmap(personnage.droite[i]);
        destroy_bitmap(personnage.haut[i]);

        destroy_bitmap(personnage_deux.bas[i]);
        destroy_bitmap(personnage_deux.gauche[i]);
        destroy_bitmap(personnage_deux.droite[i]);
        destroy_bitmap(personnage_deux.haut[i]);
    }

    destroy_bitmap(sprite);
    destroy_bitmap(sprite_deux);
    destroy_bitmap(imageChargee);
    destroy_bitmap(buffer);

    return 0;
}

void initialisationAllegro()
{
    allegro_init();
    set_color_depth(desktop_color_depth());
    if ((set_gfx_mode(GFX_AUTODETECT_WINDOWED, 632, 632, 0, 0)) != 0)
    {
        allegro_message("Pb de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

void separer_bitmap_personnage_un(t_personnage *personnage, BITMAP *bitmap)
{
    int taille_w = 48;
    int taille_h = 48;

    for (int j = 0; j < 3; j++)
    {
        personnage->haut[j] = create_sub_bitmap(bitmap, j * taille_w, 0 * taille_h, taille_w, taille_h);
        personnage->gauche[j] = create_sub_bitmap(bitmap, j * taille_w, 1 * taille_h, taille_w, taille_h);
        personnage->droite[j] = create_sub_bitmap(bitmap, j * taille_w, 2 * taille_h, taille_w, taille_h);
        personnage->bas[j] = create_sub_bitmap(bitmap, j * taille_w, 3 * taille_h, taille_w, taille_h);
    }
}



void dessiner_personnage(t_personnage *personnage, BITMAP *buffer)
{
    BITMAP *frame;
    switch (personnage->direction)
    {
        case 0:
            frame = personnage->haut[personnage->frame];
            break;
        case 1:
            frame = personnage->gauche[personnage->frame];
            break;
        case 2:
            frame = personnage->droite[personnage->frame];
            break;
        case 3:
            frame = personnage->bas[personnage->frame];
            break;
    }
    masked_blit(frame, buffer, 0, 0, personnage->x, personnage->y, frame->w, frame->h);
}


void maj_personnage(t_personnage *personnage)
{
    int vitesse = 1;
    personnage->en_deplacement = 0;
    if (key[KEY_UP])
    {
        personnage->y -= vitesse;
        personnage->direction = 3;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_DOWN])
    {
        personnage->y += vitesse;
        personnage->direction = 0;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_LEFT])
    {
        personnage->x -= vitesse;
        personnage->direction = 1;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_RIGHT])
    {
        personnage->x += vitesse;
        personnage->direction = 2;
        personnage->en_deplacement = 1;
    }
}

void maj_personnage_deux(t_personnage *personnage_deux)
{
    int vitesse = 1;
    personnage_deux->en_deplacement = 0;
    if (key[KEY_E])
    {
        personnage_deux->y -= vitesse;
        personnage_deux->direction = 3;
        personnage_deux->en_deplacement = 1;
    }
    if (key[KEY_D])
    {
        personnage_deux->y += vitesse;
        personnage_deux->direction = 0;
        personnage_deux->en_deplacement = 1;
    }
    if (key[KEY_S])
    {
        personnage_deux->x -= vitesse;
        personnage_deux->direction = 1;
        personnage_deux->en_deplacement = 1;
    }
    if (key[KEY_F])
    {
        personnage_deux->x += vitesse;
        personnage_deux->direction = 2;
        personnage_deux->en_deplacement = 1;
    }

}
