#include <stdio.h>
#include <string.h>

#include "types.h"



void initialize_board(Game *game);
void initialize_board_squares(Game *game);
void initialize_properties(Game *game);
void initialize_railway_stations(Game *game);
void initialize_utilities(Game *game);


int move_player(Game *game, int player_index, int dice_total);
int calculate_new_position(int current_position, int dice_total);
int has_passed_go(int current_position, int new_position);


void display_board(Game *game);
static const char *board_square_type_name(BoardSquareType square_type);




void initialize_board(Game *game)
{
    if (game == NULL)
    {
        return;
    }

    initialize_board_squares(game);
    initialize_properties(game);
    initialize_railway_stations(game);
    initialize_utilities(game);
}




void initialize_board_squares(Game *game)
{
    if (game == NULL)
    {
        return;
    }

    

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        game->board_squares[i].board_index = i;

        strcpy(game->board_squares[i].square_name, "Unknown");

        game->board_squares[i].square_type = EVENT;

        game->board_squares[i].property_index = -1;
        game->board_squares[i].railway_index = -1;
        game->board_squares[i].utility_index = -1;

        game->board_squares[i].tax_amount = 0;
    }


    

    strcpy(game->board_squares[0].square_name, "GO");
    game->board_squares[0].square_type = GO;


    

    strcpy(game->board_squares[1].square_name, "Pettah");
    game->board_squares[1].square_type = PROPERTY;
    game->board_squares[1].property_index = 0;


    strcpy(game->board_squares[2].square_name,
           "Community Development Fund");
    game->board_squares[2].square_type = EVENT;



    strcpy(game->board_squares[3].square_name, "Maradana");
    game->board_squares[3].square_type = PROPERTY;
    game->board_squares[3].property_index = 1;


    
    strcpy(game->board_squares[4].square_name, "Income Tax");
    game->board_squares[4].square_type = TAX;
    game->board_squares[4].tax_amount = 2000;


    
    strcpy(game->board_squares[5].square_name,
           "Colombo Fort Railway Station");
    game->board_squares[5].square_type = RAILWAY;
    game->board_squares[5].railway_index = 0;


   

    strcpy(game->board_squares[6].square_name, "Bambalapitiya");
    game->board_squares[6].square_type = PROPERTY;
    game->board_squares[6].property_index = 2;


    

    strcpy(game->board_squares[7].square_name,
           "National Event Card");
    game->board_squares[7].square_type = EVENT;


    
    strcpy(game->board_squares[8].square_name, "Wellawatte");
    game->board_squares[8].square_type = PROPERTY;
    game->board_squares[8].property_index = 3;


   

    strcpy(game->board_squares[9].square_name, "Mount Lavinia");
    game->board_squares[9].square_type = PROPERTY;
    game->board_squares[9].property_index = 4;


    

    strcpy(game->board_squares[10].square_name,
           "Jail / Just Visiting");
    game->board_squares[10].square_type = JAIL;


    

    strcpy(game->board_squares[11].square_name, "Nugegoda");
    game->board_squares[11].square_type = PROPERTY;
    game->board_squares[11].property_index = 5;


    

    strcpy(game->board_squares[12].square_name,
           "Ceylon Electricity Board");
    game->board_squares[12].square_type = UTILITY;
    game->board_squares[12].utility_index = 0;


    

    strcpy(game->board_squares[13].square_name, "Maharagama");
    game->board_squares[13].square_type = PROPERTY;
    game->board_squares[13].property_index = 6;



    strcpy(game->board_squares[14].square_name, "Kottawa");
    game->board_squares[14].square_type = PROPERTY;
    game->board_squares[14].property_index = 7;


    
    strcpy(game->board_squares[15].square_name,
           "Kandy Railway Station");
    game->board_squares[15].square_type = RAILWAY;
    game->board_squares[15].railway_index = 1;


    strcpy(game->board_squares[16].square_name, "Negombo");
    game->board_squares[16].square_type = PROPERTY;
    game->board_squares[16].property_index = 8;


    

    strcpy(game->board_squares[17].square_name,
           "Sri Lanka Insurance");
    game->board_squares[17].square_type = INSURANCE;


    

    strcpy(game->board_squares[18].square_name, "Katunayake");
    game->board_squares[18].square_type = PROPERTY;
    game->board_squares[18].property_index = 9;


    

    strcpy(game->board_squares[19].square_name, "Ja-Ela");
    game->board_squares[19].square_type = PROPERTY;
    game->board_squares[19].property_index = 10;


    

    strcpy(game->board_squares[20].square_name, "Free Parking");
    game->board_squares[20].square_type = FREE_PARKING;


    

    strcpy(game->board_squares[21].square_name, "Kandy City");
    game->board_squares[21].square_type = PROPERTY;
    game->board_squares[21].property_index = 11;


    

    strcpy(game->board_squares[22].square_name,
           "National Event Card");
    game->board_squares[22].square_type = EVENT;


    

    strcpy(game->board_squares[23].square_name, "Peradeniya");
    game->board_squares[23].square_type = PROPERTY;
    game->board_squares[23].property_index = 12;


    

    strcpy(game->board_squares[24].square_name, "Katugastota");
    game->board_squares[24].square_type = PROPERTY;
    game->board_squares[24].property_index = 13;


    

    strcpy(game->board_squares[25].square_name,
           "Galle Railway Station");
    game->board_squares[25].square_type = RAILWAY;
    game->board_squares[25].railway_index = 2;


    

    strcpy(game->board_squares[26].square_name, "Galle Fort");
    game->board_squares[26].square_type = PROPERTY;
    game->board_squares[26].property_index = 14;


    

    strcpy(game->board_squares[27].square_name, "Unawatuna");
    game->board_squares[27].square_type = PROPERTY;
    game->board_squares[27].property_index = 15;


    

    strcpy(game->board_squares[28].square_name,
           "National Water Supply and Drainage Board");
    game->board_squares[28].square_type = UTILITY;
    game->board_squares[28].utility_index = 1;


    

    strcpy(game->board_squares[29].square_name, "Hikkaduwa");
    game->board_squares[29].square_type = PROPERTY;
    game->board_squares[29].property_index = 16;


    

    strcpy(game->board_squares[30].square_name, "Go To Jail");
    game->board_squares[30].square_type = GO_TO_JAIL;


    

    strcpy(game->board_squares[31].square_name, "Jaffna Town");
    game->board_squares[31].square_type = PROPERTY;
    game->board_squares[31].property_index = 17;


    

    strcpy(game->board_squares[32].square_name, "Nallur");
    game->board_squares[32].square_type = PROPERTY;
    game->board_squares[32].property_index = 18;


    

    strcpy(game->board_squares[33].square_name,
           "Ceylinco Insurance");
    game->board_squares[33].square_type = INSURANCE;


    

    strcpy(game->board_squares[34].square_name, "Trincomalee");
    game->board_squares[34].square_type = PROPERTY;
    game->board_squares[34].property_index = 19;


    

    strcpy(game->board_squares[35].square_name,
           "Jaffna Railway Station");
    game->board_squares[35].square_type = RAILWAY;
    game->board_squares[35].railway_index = 3;


    

    strcpy(game->board_squares[36].square_name,
           "National Event Card");
    game->board_squares[36].square_type = EVENT;


    

    strcpy(game->board_squares[37].square_name, "Nuwara Eliya");
    game->board_squares[37].square_type = PROPERTY;
    game->board_squares[37].property_index = 20;


    

    strcpy(game->board_squares[38].square_name,
           "Bank of Ceylon");
    game->board_squares[38].square_type = BANK;


    

    strcpy(game->board_squares[39].square_name, "Galle Face");
    game->board_squares[39].square_type = PROPERTY;
    game->board_squares[39].property_index = 21;
}




void initialize_properties(Game *game)
{
    if (game == NULL)
    {
        return;
    }


    

    strcpy(game->properties[0].property_name, "Pettah");

    game->properties[0].board_index = 1;
    game->properties[0].property_group = BROWN;

    game->properties[0].purchase_price = 1500;
    game->properties[0].base_rent = 100;
    game->properties[0].house_construction_cost = 500;
    game->properties[0].hotel_construction_cost = 2000;
    game->properties[0].mortgage_value = 750;


    

    strcpy(game->properties[1].property_name, "Maradana");

    game->properties[1].board_index = 3;
    game->properties[1].property_group = BROWN;

    game->properties[1].purchase_price = 1800;
    game->properties[1].base_rent = 120;
    game->properties[1].house_construction_cost = 500;
    game->properties[1].hotel_construction_cost = 2000;
    game->properties[1].mortgage_value = 900;


    

    strcpy(game->properties[2].property_name, "Bambalapitiya");

    game->properties[2].board_index = 6;
    game->properties[2].property_group = LIGHT_BLUE;

    game->properties[2].purchase_price = 2500;
    game->properties[2].base_rent = 180;
    game->properties[2].house_construction_cost = 750;
    game->properties[2].hotel_construction_cost = 3000;
    game->properties[2].mortgage_value = 1250;


    

    strcpy(game->properties[3].property_name, "Wellawatte");

    game->properties[3].board_index = 8;
    game->properties[3].property_group = LIGHT_BLUE;

    game->properties[3].purchase_price = 2700;
    game->properties[3].base_rent = 200;
    game->properties[3].house_construction_cost = 750;
    game->properties[3].hotel_construction_cost = 3000;
    game->properties[3].mortgage_value = 1350;


    

    strcpy(game->properties[4].property_name, "Mount Lavinia");

    game->properties[4].board_index = 9;
    game->properties[4].property_group = LIGHT_BLUE;

    game->properties[4].purchase_price = 3000;
    game->properties[4].base_rent = 220;
    game->properties[4].house_construction_cost = 750;
    game->properties[4].hotel_construction_cost = 3000;
    game->properties[4].mortgage_value = 1500;


    

    strcpy(game->properties[5].property_name, "Nugegoda");

    game->properties[5].board_index = 11;
    game->properties[5].property_group = PINK;

    game->properties[5].purchase_price = 3500;
    game->properties[5].base_rent = 260;
    game->properties[5].house_construction_cost = 1000;
    game->properties[5].hotel_construction_cost = 4000;
    game->properties[5].mortgage_value = 1750;


    

    strcpy(game->properties[6].property_name, "Maharagama");

    game->properties[6].board_index = 13;
    game->properties[6].property_group = PINK;

    game->properties[6].purchase_price = 3800;
    game->properties[6].base_rent = 280;
    game->properties[6].house_construction_cost = 1000;
    game->properties[6].hotel_construction_cost = 4000;
    game->properties[6].mortgage_value = 1900;


    

    strcpy(game->properties[7].property_name, "Kottawa");

    game->properties[7].board_index = 14;
    game->properties[7].property_group = PINK;

    game->properties[7].purchase_price = 4000;
    game->properties[7].base_rent = 300;
    game->properties[7].house_construction_cost = 1000;
    game->properties[7].hotel_construction_cost = 4000;
    game->properties[7].mortgage_value = 2000;


    

    strcpy(game->properties[8].property_name, "Negombo");

    game->properties[8].board_index = 16;
    game->properties[8].property_group = ORANGE;

    game->properties[8].purchase_price = 4500;
    game->properties[8].base_rent = 350;
    game->properties[8].house_construction_cost = 1250;
    game->properties[8].hotel_construction_cost = 5000;
    game->properties[8].mortgage_value = 2250;


    

    strcpy(game->properties[9].property_name, "Katunayake");

    game->properties[9].board_index = 18;
    game->properties[9].property_group = ORANGE;

    game->properties[9].purchase_price = 4700;
    game->properties[9].base_rent = 370;
    game->properties[9].house_construction_cost = 1250;
    game->properties[9].hotel_construction_cost = 5000;
    game->properties[9].mortgage_value = 2350;


    

    strcpy(game->properties[10].property_name, "Ja-Ela");

    game->properties[10].board_index = 19;
    game->properties[10].property_group = ORANGE;

    game->properties[10].purchase_price = 5000;
    game->properties[10].base_rent = 400;
    game->properties[10].house_construction_cost = 1250;
    game->properties[10].hotel_construction_cost = 5000;
    game->properties[10].mortgage_value = 2500;


    

    strcpy(game->properties[11].property_name, "Kandy City");

    game->properties[11].board_index = 21;
    game->properties[11].property_group = RED;

    game->properties[11].purchase_price = 5500;
    game->properties[11].base_rent = 450;
    game->properties[11].house_construction_cost = 1500;
    game->properties[11].hotel_construction_cost = 6000;
    game->properties[11].mortgage_value = 2750;


    

    strcpy(game->properties[12].property_name, "Peradeniya");

    game->properties[12].board_index = 23;
    game->properties[12].property_group = RED;

    game->properties[12].purchase_price = 5800;
    game->properties[12].base_rent = 480;
    game->properties[12].house_construction_cost = 1500;
    game->properties[12].hotel_construction_cost = 6000;
    game->properties[12].mortgage_value = 2900;


    

    strcpy(game->properties[13].property_name, "Katugastota");

    game->properties[13].board_index = 24;
    game->properties[13].property_group = RED;

    game->properties[13].purchase_price = 6000;
    game->properties[13].base_rent = 500;
    game->properties[13].house_construction_cost = 1500;
    game->properties[13].hotel_construction_cost = 6000;
    game->properties[13].mortgage_value = 3000;


    

    strcpy(game->properties[14].property_name, "Galle Fort");

    game->properties[14].board_index = 26;
    game->properties[14].property_group = YELLOW;

    game->properties[14].purchase_price = 6500;
    game->properties[14].base_rent = 600;
    game->properties[14].house_construction_cost = 2000;
    game->properties[14].hotel_construction_cost = 8000;
    game->properties[14].mortgage_value = 3250;


    

    strcpy(game->properties[15].property_name, "Unawatuna");

    game->properties[15].board_index = 27;
    game->properties[15].property_group = YELLOW;

    game->properties[15].purchase_price = 6800;
    game->properties[15].base_rent = 620;
    game->properties[15].house_construction_cost = 2000;
    game->properties[15].hotel_construction_cost = 8000;
    game->properties[15].mortgage_value = 3400;


    

    strcpy(game->properties[16].property_name, "Hikkaduwa");

    game->properties[16].board_index = 29;
    game->properties[16].property_group = YELLOW;

    game->properties[16].purchase_price = 7000;
    game->properties[16].base_rent = 650;
    game->properties[16].house_construction_cost = 2000;
    game->properties[16].hotel_construction_cost = 8000;
    game->properties[16].mortgage_value = 3500;


    

    strcpy(game->properties[17].property_name, "Jaffna Town");

    game->properties[17].board_index = 31;
    game->properties[17].property_group = GREEN;

    game->properties[17].purchase_price = 8000;
    game->properties[17].base_rent = 750;
    game->properties[17].house_construction_cost = 2500;
    game->properties[17].hotel_construction_cost = 10000;
    game->properties[17].mortgage_value = 4000;


    

    strcpy(game->properties[18].property_name, "Nallur");

    game->properties[18].board_index = 32;
    game->properties[18].property_group = GREEN;

    game->properties[18].purchase_price = 8300;
    game->properties[18].base_rent = 780;
    game->properties[18].house_construction_cost = 2500;
    game->properties[18].hotel_construction_cost = 10000;
    game->properties[18].mortgage_value = 4150;


    

    strcpy(game->properties[19].property_name, "Trincomalee");

    game->properties[19].board_index = 34;
    game->properties[19].property_group = GREEN;

    game->properties[19].purchase_price = 8500;
    game->properties[19].base_rent = 800;
    game->properties[19].house_construction_cost = 2500;
    game->properties[19].hotel_construction_cost = 10000;
    game->properties[19].mortgage_value = 4250;


    

    strcpy(game->properties[20].property_name, "Nuwara Eliya");

    game->properties[20].board_index = 37;
    game->properties[20].property_group = DARK_BLUE;

    game->properties[20].purchase_price = 10000;
    game->properties[20].base_rent = 1000;
    game->properties[20].house_construction_cost = 3000;
    game->properties[20].hotel_construction_cost = 12000;
    game->properties[20].mortgage_value = 5000;


    

    strcpy(game->properties[21].property_name, "Galle Face");

    game->properties[21].board_index = 39;
    game->properties[21].property_group = DARK_BLUE;

    game->properties[21].purchase_price = 12000;
    game->properties[21].base_rent = 1200;
    game->properties[21].house_construction_cost = 3000;
    game->properties[21].hotel_construction_cost = 12000;
    game->properties[21].mortgage_value = 6000;


    

    for (int i = 0; i < NUMBER_OF_PROPERTIES; i++)
    {
        game->properties[i].current_property_value =
            game->properties[i].purchase_price;

        game->properties[i].current_mortgage_value =
            game->properties[i].mortgage_value;

        game->properties[i].current_rent =
            game->properties[i].base_rent;

        game->properties[i].current_house_construction_cost =
            game->properties[i].house_construction_cost;

        game->properties[i].current_hotel_construction_cost =
            game->properties[i].hotel_construction_cost;

        
        game->properties[i].owner_player_index = BANK_OWNER;

        game->properties[i].is_mortgaged = 0;

        game->properties[i].number_of_houses = 0;
        game->properties[i].has_hotel = 0;

        game->properties[i].property_age = 0;
        game->properties[i].depreciation_percentage = 0;

        game->properties[i].is_damaged = 0;
        game->properties[i].repair_cost = 0;


        
        game->properties[i].insurance_policy.insurance_type =
            NO_INSURANCE;

        game->properties[i].insurance_policy.property_index = i;

        game->properties[i].insurance_policy.premium = 0;

        game->properties[i].insurance_policy.rounds_remaining = 0;

        game->properties[i].insurance_policy.is_active = 0;


        
        for (int j = 0; j < 5; j++)
        {
            game->properties[i].buildings[j].building_type =
                NO_BUILDING;

            game->properties[i].buildings[j].condition = 100;
        }
    }
}




void initialize_railway_stations(Game *game)
{
    if (game == NULL)
    {
        return;
    }


    strcpy(game->railway_stations[0].railway_name,
           "Colombo Fort Railway Station");

    game->railway_stations[0].board_index = 5;


    strcpy(game->railway_stations[1].railway_name,
           "Kandy Railway Station");

    game->railway_stations[1].board_index = 15;


    strcpy(game->railway_stations[2].railway_name,
           "Galle Railway Station");

    game->railway_stations[2].board_index = 25;


    strcpy(game->railway_stations[3].railway_name,
           "Jaffna Railway Station");

    game->railway_stations[3].board_index = 35;


    for (int i = 0; i < NUMBER_OF_RAILWAY_STATIONS; i++)
    {
        
        game->railway_stations[i].owner_player_index = BANK_OWNER;

        game->railway_stations[i].mortgage_value = 0;
        game->railway_stations[i].current_value = 0;

        game->railway_stations[i].is_mortgaged = 0;
    }
}




void initialize_utilities(Game *game)
{
    if (game == NULL)
    {
        return;
    }


    strcpy(game->utility_companies[0].utility_name,
           "Ceylon Electricity Board");

    game->utility_companies[0].board_index = 12;


    strcpy(game->utility_companies[1].utility_name,
           "National Water Supply and Drainage Board");

    game->utility_companies[1].board_index = 28;


    for (int i = 0; i < NUMBER_OF_UTILITY_COMPANIES; i++)
    {
        game->utility_companies[i].owner_player_index = BANK_OWNER;

        game->utility_companies[i].mortgage_value = 0;
        game->utility_companies[i].current_value = 0;

        game->utility_companies[i].is_mortgaged = 0;
    }
}




int calculate_new_position(int current_position, int dice_total)
{
    int new_position;

    new_position = current_position + dice_total;

    
    new_position = new_position % BOARD_SIZE;

    return new_position;
}




int has_passed_go(int current_position, int new_position)
{
    

    if (new_position < current_position)
    {
        return 1;
    }

    return 0;
}




int move_player(Game *game, int player_index, int dice_total)
{
    int old_position;
    int new_position;

    if (game == NULL)
    {
        return -1;
    }

    if (player_index < 0 || player_index >= NUMBER_OF_PLAYERS)
    {
        return -1;
    }

    if (dice_total < 0)
    {
        return -1;
    }


    old_position =
        game->players[player_index].current_board_position;


    new_position =
        calculate_new_position(old_position, dice_total);


    
    game->players[player_index].previous_board_position =
        old_position;


    
    game->players[player_index].current_board_position =
        new_position;


    return new_position;
}



static const char *board_square_type_name(BoardSquareType square_type)
{
    switch (square_type)
    {
        case GO:            return "GO";
        case PROPERTY:       return "Property";
        case EVENT:          return "Event";
        case TAX:            return "Tax";
        case RAILWAY:        return "Railway";
        case UTILITY:        return "Utility";
        case JAIL:           return "Jail";
        case FREE_PARKING:   return "Free Parking";
        case GO_TO_JAIL:     return "Go To Jail";
        case INSURANCE:      return "Insurance";
        case BANK:           return "Bank";
        default:             return "Unknown";
    }
}




void display_board(Game *game)
{
    int i;

    if (game == NULL)
    {
        return;
    }

    printf("%-6s %-14s %-30s\n", "Sq", "Type", "Name");
    printf("--------------------------------------------------\n");

    for (i = 0; i < BOARD_SIZE; i++)
    {
        BoardSquare *square = &game->board_squares[i];

        printf(
            "%-6d %-14s %-30s\n",
            square->board_index,
            board_square_type_name(square->square_type),
            square->square_name
        );
    }

    printf("--------------------------------------------------\n");
}
