#include <stdio.h>
#include <stdlib.h>
#include "types.h"









static int is_valid_player(Game *game, int player_index)
{
    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (game->players[player_index].player_status !=
        ACTIVE_PLAYER)
        return 0;

    return 1;
}





static int positive_value(int value)
{
    if (value < 0)
        return 0;

    return value;
}









void set_economic_condition(Game *game,
                            EconomicCondition condition)
{
    if (game == NULL)
        return;

    game->current_economic_condition = condition;


    switch (condition)
    {
        case STABLE_ECONOMY:

            game->current_inflation.inflation_percentage = 0;

            game->current_loan_interest_rate = 5;

            break;


        case ECONOMIC_BOOM:

            game->current_inflation.inflation_percentage = 2;

            game->current_loan_interest_rate = 4;

            break;


        case MODERATE_INFLATION:

            game->current_inflation.inflation_percentage = 5;

            game->current_loan_interest_rate = 6;

            break;


        case HIGH_INFLATION:

            game->current_inflation.inflation_percentage = 10;

            game->current_loan_interest_rate = 8;

            break;


        case ECONOMIC_RECESSION:

            game->current_inflation.inflation_percentage = -5;

            game->current_loan_interest_rate = 10;

            break;


        default:

            break;
    }
}






void apply_economic_condition(Game *game)
{
    int i;
    int inflation;

    if (game == NULL)
        return;

    inflation =
        game->current_inflation.inflation_percentage;


    for (i = 0;
         i < NUMBER_OF_PROPERTIES;
         i++)
    {
        Property *property =
            &game->properties[i];


        property->current_property_value =
            property->purchase_price *
            (100 + inflation) / 100;


        property->current_rent =
            property->base_rent *
            (100 + inflation) / 100;


        property->current_house_construction_cost =
            property->house_construction_cost *
            (100 + inflation) / 100;


        property->current_hotel_construction_cost =
            property->hotel_construction_cost *
            (100 + inflation) / 100;






        property->current_property_value =
            positive_value(
                property->current_property_value
            );

        property->current_rent =
            positive_value(
                property->current_rent
            );

        property->current_house_construction_cost =
            positive_value(
                property->current_house_construction_cost
            );

        property->current_hotel_construction_cost =
            positive_value(
                property->current_hotel_construction_cost
            );
    }
}









void increase_inflation(Game *game,
                        int percentage)
{
    if (game == NULL)
        return;

    if (percentage < 0)
        return;

    game->current_inflation.inflation_percentage +=
        percentage;

    apply_economic_condition(game);
}





void decrease_inflation(Game *game,
                        int percentage)
{
    if (game == NULL)
        return;

    if (percentage < 0)
        return;

    game->current_inflation.inflation_percentage -=
        percentage;

    if (game->current_inflation.inflation_percentage < 0)
    {
        game->current_inflation.inflation_percentage = 0;
    }

    apply_economic_condition(game);
}









void start_economic_event(Game *game,
                          EconomicEventType event_type,
                          int duration)
{
    if (game == NULL)
        return;

    if (duration <= 0)
        return;

    game->current_economic_event.economic_event_type =
        event_type;

    game->current_economic_event.rounds_remaining =
        duration;

    game->current_economic_event.is_active = 1;


    switch (event_type)
    {
        case TOURISM_BOOM:

            set_economic_condition(
                game,
                ECONOMIC_BOOM
            );

            break;


        case FUEL_CRISIS:

            increase_inflation(game, 5);

            break;


        case HEAVY_MONSOON:

            increase_inflation(game, 2);

            break;


        case ECONOMIC_RECESSION_EVENT:

            set_economic_condition(
                game,
                ECONOMIC_RECESSION
            );

            break;


        case STOCK_MARKET_BOOM:

            increase_inflation(game, 3);

            break;


        case GOVERNMENT_HOUSING_PROGRAMME:

            decrease_inflation(game, 2);

            break;


        case FOREIGN_INVESTMENT:

            set_economic_condition(
                game,
                ECONOMIC_BOOM
            );

            break;


        case POLITICAL_UNREST:

            increase_inflation(game, 4);

            break;


        default:

            break;
    }
}





void update_economic_event(Game *game)
{
    if (game == NULL)
        return;

    if (!game->current_economic_event.is_active)
        return;

    if (game->current_economic_event.rounds_remaining > 0)
    {
        game->current_economic_event.rounds_remaining--;
    }

    if (game->current_economic_event.rounds_remaining <= 0)
    {
        game->current_economic_event.is_active = 0;

        game->current_economic_event.rounds_remaining = 0;

        set_economic_condition(
            game,
            STABLE_ECONOMY
        );
    }
}









void start_government_regulation(
    Game *game,
    GovernmentRegulationType regulation_type,
    int duration)
{
    if (game == NULL)
        return;

    if (duration <= 0)
        return;

    game->current_government_regulation
        .government_regulation_type =
        regulation_type;

    game->current_government_regulation
        .rounds_remaining =
        duration;

    game->current_government_regulation
        .is_active = 1;


    switch (regulation_type)
    {
        case INCREASE_PROPERTY_TAX:

            break;


        case REDUCE_LOAN_INTEREST:

            if (game->current_loan_interest_rate > 1)
            {
                game->current_loan_interest_rate--;
            }

            break;


        case HOUSING_SUBSIDY:

            break;


        case LUXURY_PROPERTY_TAX:

            break;


        case RAILWAY_MODERNIZATION:

            break;


        case ELECTRICITY_TARIFF_REVISION:

            break;


        case INSURANCE_REGULATION:

            break;


        case ANTI_SPECULATION_ACT:

            break;


        default:

            break;
    }
}





void update_government_regulation(Game *game)
{
    if (game == NULL)
        return;

    if (!game->current_government_regulation.is_active)
        return;

    if (game->current_government_regulation.rounds_remaining > 0)
    {
        game->current_government_regulation
            .rounds_remaining--;
    }

    if (game->current_government_regulation.rounds_remaining <= 0)
    {
        game->current_government_regulation.is_active = 0;

        game->current_government_regulation
            .rounds_remaining = 0;
    }
}









void start_regional_development(
    Game *game,
    RegionalDevelopmentType development_type,
    int duration)
{
    if (game == NULL)
        return;

    if (duration <= 0)
        return;

    game->current_regional_development
        .regional_development_type =
        development_type;

    game->current_regional_development
        .rounds_remaining =
        duration;

    game->current_regional_development
        .is_active = 1;


    switch (development_type)
    {
        case SOUTHERN_TOURISM_BOOM:

            game->property_market.boom_property_group =
                ORANGE;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case PORT_CITY_EXPANSION:

            game->property_market.boom_property_group =
                DARK_BLUE;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case IT_INDUSTRY_GROWTH:

            game->property_market.boom_property_group =
                LIGHT_BLUE;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case NORTHERN_DEVELOPMENT_PROGRAMME:

            game->property_market.boom_property_group =
                GREEN;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case TEA_EXPORT_BOOM:

            game->property_market.boom_property_group =
                BROWN;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case AIRPORT_EXPANSION:

            game->property_market.boom_property_group =
                YELLOW;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case UNIVERSITY_CITY_GROWTH:

            game->property_market.boom_property_group =
                PINK;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                duration;

            break;


        case BEACH_POLLUTION:

            game->property_market.decline_property_group =
                ORANGE;

            game->property_market.decline_is_active = 1;

            game->property_market.decline_rounds_remaining =
                duration;

            break;


        case FLOOD_DAMAGE:

            game->property_market.decline_property_group =
                GREEN;

            game->property_market.decline_is_active = 1;

            game->property_market.decline_rounds_remaining =
                duration;

            break;


        case TRANSPORT_STRIKE:

            game->property_market.decline_property_group =
                YELLOW;

            game->property_market.decline_is_active = 1;

            game->property_market.decline_rounds_remaining =
                duration;

            break;


        case ELECTRICITY_TARIFF_INCREASE:

            increase_inflation(game, 3);

            break;


        case WATER_SHORTAGE:

            increase_inflation(game, 2);

            break;


        default:

            break;
    }
}





void update_regional_development(Game *game)
{
    if (game == NULL)
        return;


    if (game->current_regional_development.is_active)
    {
        if (game->current_regional_development
                .rounds_remaining > 0)
        {
            game->current_regional_development
                .rounds_remaining--;
        }


        if (game->current_regional_development
                .rounds_remaining <= 0)
        {
            game->current_regional_development
                .is_active = 0;

            game->current_regional_development
                .rounds_remaining = 0;
        }
    }


    if (game->property_market.boom_is_active)
    {
        if (game->property_market
                .boom_rounds_remaining > 0)
        {
            game->property_market
                .boom_rounds_remaining--;
        }


        if (game->property_market
                .boom_rounds_remaining <= 0)
        {
            game->property_market
                .boom_is_active = 0;
        }
    }


    if (game->property_market.decline_is_active)
    {
        if (game->property_market
                .decline_rounds_remaining > 0)
        {
            game->property_market
                .decline_rounds_remaining--;
        }


        if (game->property_market
                .decline_rounds_remaining <= 0)
        {
            game->property_market
                .decline_is_active = 0;
        }
    }
}









void apply_property_market(Game *game)
{
    int i;

    if (game == NULL)
        return;


    for (i = 0;
         i < NUMBER_OF_PROPERTIES;
         i++)
    {
        Property *property =
            &game->properties[i];






        if (game->property_market.boom_is_active &&
            property->property_group ==
            game->property_market.boom_property_group)
        {
            property->current_property_value =
                property->purchase_price * 120 / 100;

            property->current_rent =
                property->base_rent * 120 / 100;
        }






        if (game->property_market.decline_is_active &&
            property->property_group ==
            game->property_market.decline_property_group)
        {
            property->current_property_value =
                property->purchase_price * 80 / 100;

            property->current_rent =
                property->base_rent * 80 / 100;
        }
    }
}









void apply_disaster(Game *game,
                    int property_index,
                    DisasterType disaster_type)
{
    Property *property;

    if (game == NULL)
        return;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return;

    property =
        &game->properties[property_index];


    switch (disaster_type)
    {
        case FIRE:

            property->is_damaged = 1;

            property->repair_cost =
                property->current_property_value * 25 / 100;

            break;


        case FLOOD:

            property->is_damaged = 1;

            property->repair_cost =
                property->current_property_value * 20 / 100;

            break;


        case RIOT:

            property->is_damaged = 1;

            property->repair_cost =
                property->current_property_value * 30 / 100;

            break;


        case BUILDING_COLLAPSE:

            property->is_damaged = 1;

            property->repair_cost =
                property->current_property_value * 40 / 100;

            property->number_of_houses = 0;

            property->has_hotel = 0;

            break;


        case ELECTRICAL_FAILURE:

            property->is_damaged = 1;

            property->repair_cost =
                property->current_property_value * 15 / 100;

            break;


        case NO_DISASTER:

        default:

            break;
    }
}





int repair_property(Game *game,
                    int player_index,
                    int property_index)
{
    Player *player;
    Property *property;

    if (game == NULL)
        return 0;

    if (!is_valid_player(game, player_index))
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player =
        &game->players[player_index];

    property =
        &game->properties[property_index];


    if (property->owner_player_index !=
        player_index)
        return 0;

    if (!property->is_damaged)
        return 0;

    if (player->cash < property->repair_cost)
        return 0;

    player->cash -=
        property->repair_cost;

    property->is_damaged = 0;

    property->repair_cost = 0;

    return 1;
}









void initialize_national_event_cards(Game *game)
{
    int i;

    if (game == NULL)
        return;


    for (i = 0;
         i < NUMBER_OF_NATIONAL_EVENT_CARDS;
         i++)
    {
        game->national_event_cards[i] =
            (NationalEventCardType)i;
    }


    game->current_national_event_card_index = 0;
}





NationalEventCardType draw_national_event_card(
    Game *game)
{
    NationalEventCardType card;

    if (game == NULL)
        return NATIONAL_DISASTER;


    if (game->current_national_event_card_index >=
        NUMBER_OF_NATIONAL_EVENT_CARDS)
    {
        game->current_national_event_card_index = 0;
    }


    card =
        game->national_event_cards[
            game->current_national_event_card_index
        ];


    game->current_national_event_card_index++;

    return card;
}





void apply_national_event_card(
    Game *game,
    NationalEventCardType card)
{
    int i;

    if (game == NULL)
        return;


    switch (card)
    {




        case TOURISM_HYPE:

            increase_inflation(game, 3);

            break;






        case FUEL_SHORTAGE:

            increase_inflation(game, 5);

            break;






        case HEAVY_FLOODS:

            for (i = 0;
                 i < NUMBER_OF_PROPERTIES;
                 i++)
            {
                if (game->properties[i]
                        .owner_player_index >= 0)
                {
                    apply_disaster(
                        game,
                        i,
                        FLOOD
                    );
                }
            }

            break;






        case POLITICAL_RALLY:

            increase_inflation(game, 2);

            break;






        case STOCK_MARKET_RISE:

            set_economic_condition(
                game,
                ECONOMIC_BOOM
            );

            break;






        case ECONOMIC_DOWNTURN:

            set_economic_condition(
                game,
                ECONOMIC_RECESSION
            );

            break;






        case HOUSING_SUBSIDY_CARD:

            for (i = 0;
                 i < NUMBER_OF_PROPERTIES;
                 i++)
            {
                game->properties[i]
                    .current_house_construction_cost =
                    game->properties[i]
                    .house_construction_cost * 75 / 100;
            }

            break;






        case INTEREST_RATE_CUT:

            if (game->current_loan_interest_rate > 1)
            {
                game->current_loan_interest_rate -= 2;

                if (game->current_loan_interest_rate < 1)
                {
                    game->current_loan_interest_rate = 1;
                }
            }

            break;






        case INTEREST_RATE_INCREASE:

            game->current_loan_interest_rate += 2;

            break;






        case TAX_AMNESTY:

            for (i = 0;
                 i < NUMBER_OF_PLAYERS;
                 i++)
            {
                game->players[i].taxes_due = 0;
            }

            break;






        case POWER_FAILURE:

            for (i = 0;
                 i < NUMBER_OF_PROPERTIES;
                 i++)
            {
                if (game->properties[i]
                        .owner_player_index >= 0)
                {
                    apply_disaster(
                        game,
                        i,
                        ELECTRICAL_FAILURE
                    );
                }
            }

            break;






        case FOREIGN_FUNDING:

            for (i = 0;
                 i < NUMBER_OF_PLAYERS;
                 i++)
            {
                if (is_valid_player(game, i))
                {
                    game->players[i].cash += 1000;
                }
            }

            break;






        case PORT_EXPANSION:

            game->property_market.boom_property_group =
                DARK_BLUE;

            game->property_market.boom_is_active = 1;

            game->property_market.boom_rounds_remaining =
                5;

            break;






        case FESTIVAL_SEASON:

            increase_inflation(game, 2);

            break;






        case LABOUR_STRIKE:

            increase_inflation(game, 3);

            break;






        case INSURANCE_DISCOUNT:








            break;






        case PROPERTY_REVALUATION:

            for (i = 0;
                 i < NUMBER_OF_PROPERTIES;
                 i++)
            {
                game->properties[i]
                    .current_property_value =
                    game->properties[i]
                    .current_property_value * 110 / 100;
            }

            break;






        case CURRENCY_DEPRECIATION:

            increase_inflation(game, 8);

            break;






        case GOVERNMENT_GRANT:

            for (i = 0;
                 i < NUMBER_OF_PLAYERS;
                 i++)
            {
                if (is_valid_player(game, i))
                {
                    game->players[i].cash += 2000;
                }
            }

            break;






        case NATIONAL_DISASTER:

            for (i = 0;
                 i < NUMBER_OF_PROPERTIES;
                 i++)
            {
                if (game->properties[i]
                        .owner_player_index >= 0)
                {
                    apply_disaster(
                        game,
                        i,
                        FLOOD
                    );
                }
            }

            increase_inflation(game, 5);

            break;


        default:

            break;
    }
}









void update_all_events(Game *game)
{
    if (game == NULL)
        return;

    update_economic_event(game);

    update_government_regulation(game);

    update_regional_development(game);

    apply_property_market(game);
}