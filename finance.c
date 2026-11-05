#include <stdio.h>
#include "types.h"









static int add_property_to_player(Player *player,
                                  int property_index)
{
    if (player == NULL)
        return 0;

    if (player->number_of_owned_properties >=
        MAX_PROPERTIES_PER_PLAYER)
        return 0;

    player->owned_property_indices[
        player->number_of_owned_properties
    ] = property_index;

    player->number_of_owned_properties++;

    return 1;
}





static int remove_property_from_player(Player *player,
                                       int property_index)
{
    int i;

    if (player == NULL)
        return 0;

    for (i = 0;
         i < player->number_of_owned_properties;
         i++)
    {
        if (player->owned_property_indices[i] ==
            property_index)
        {
            for (; i <
                   player->number_of_owned_properties - 1;
                   i++)
            {
                player->owned_property_indices[i] =
                    player->owned_property_indices[i + 1];
            }

            player->number_of_owned_properties--;

            return 1;
        }
    }

    return 0;
}






int purchase_property(Game *game,
                      int player_index,
                      int property_index)
{
    Player *player;
    Property *property;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player = &game->players[player_index];
    property = &game->properties[property_index];

    if (player->player_status != ACTIVE_PLAYER)
        return 0;

    if (property->owner_player_index != BANK_OWNER)
        return 0;

    if (player->cash < property->current_property_value)
        return 0;

    if (!add_property_to_player(player, property_index))
        return 0;

    player->cash -= property->current_property_value;

    property->owner_player_index = player_index;

    return 1;
}






int calculate_property_rent(Property *property)
{
    int multiplier;

    if (property == NULL)
        return 0;

    if (property->is_mortgaged)
        return 0;

    if (property->is_damaged)
        return 0;

    if (property->has_hotel)
    {
        multiplier = 10;
    }
    else
    {
        switch (property->number_of_houses)
        {
            case 0:
                multiplier = 1;
                break;

            case 1:
                multiplier = 2;
                break;

            case 2:
                multiplier = 3;
                break;

            case 3:
                multiplier = 5;
                break;

            case 4:
                multiplier = 7;
                break;

            default:
                multiplier = 1;
                break;
        }
    }

    return property->current_rent * multiplier;
}






int pay_property_rent(Game *game,
                      int payer_index,
                      int property_index)
{
    Property *property;
    Player *payer;
    Player *owner;
    int rent;

    if (game == NULL)
        return 0;

    if (payer_index < 0 ||
        payer_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    property = &game->properties[property_index];

    if (property->owner_player_index < 0 ||
        property->owner_player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property->owner_player_index == payer_index)
        return 1;

    payer = &game->players[payer_index];

    owner = &game->players[
        property->owner_player_index
    ];

    rent = calculate_property_rent(property);

    if (rent <= 0)
        return 1;

    if (payer->cash < rent)
        return 0;

    payer->cash -= rent;
    owner->cash += rent;

    return 1;
}






int calculate_railway_rent(Game *game,
                           int railway_index)
{
    RailwayStation *railway;
    int owner_index;
    int number_owned;

    if (game == NULL)
        return 0;

    if (railway_index < 0 ||
        railway_index >= NUMBER_OF_RAILWAY_STATIONS)
        return 0;

    railway = &game->railway_stations[railway_index];

    if (railway->is_mortgaged)
        return 0;

    owner_index = railway->owner_player_index;

    if (owner_index < 0 ||
        owner_index >= NUMBER_OF_PLAYERS)
        return 0;

    number_owned =
        game->players[owner_index].number_of_owned_railways;

    switch (number_owned)
    {
        case 1:
            return 250;

        case 2:
            return 500;

        case 3:
            return 1000;

        case 4:
            return 2000;

        default:
            return 0;
    }
}






int calculate_utility_rent(Game *game,
                           int utility_index,
                           int dice_total)
{
    UtilityCompany *utility;
    int owner_index;
    int number_owned;

    if (game == NULL)
        return 0;

    if (utility_index < 0 ||
        utility_index >= NUMBER_OF_UTILITY_COMPANIES)
        return 0;

    if (dice_total < 0)
        return 0;

    utility = &game->utility_companies[utility_index];

    if (utility->is_mortgaged)
        return 0;

    owner_index = utility->owner_player_index;

    if (owner_index < 0 ||
        owner_index >= NUMBER_OF_PLAYERS)
        return 0;

    number_owned =
        game->players[owner_index].number_of_owned_utilities;

    if (number_owned == 1)
        return 4 * dice_total;

    if (number_owned == 2)
        return 10 * dice_total;

    return 0;
}






int owns_complete_property_group(Game *game,
                                 int player_index,
                                 PropertyGroup group)
{
    int total_properties = 0;
    int owned_properties = 0;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    for (i = 0; i < NUMBER_OF_PROPERTIES; i++)
    {
        if (game->properties[i].property_group == group)
        {
            total_properties++;

            if (game->properties[i].owner_player_index ==
                player_index)
            {
                owned_properties++;
            }
        }
    }

    if (total_properties == 0)
        return 0;

    return total_properties == owned_properties;
}






int build_house(Game *game,
                int player_index,
                int property_index)
{
    Player *player;
    Property *property;
    int cost;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player = &game->players[player_index];

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    if (!owns_complete_property_group(
            game,
            player_index,
            property->property_group))
        return 0;

    if (property->is_mortgaged)
        return 0;

    if (property->has_hotel)
        return 0;

    if (property->number_of_houses >=
        MAX_HOUSES_PER_PROPERTY)
        return 0;

    cost = property->current_house_construction_cost;

    if (player->cash < cost)
        return 0;

    player->cash -= cost;

    property->number_of_houses++;

    property->buildings[
        property->number_of_houses - 1
    ].building_type = HOUSE;

    property->buildings[
        property->number_of_houses - 1
    ].condition = 100;

    return 1;
}






int build_hotel(Game *game,
                int player_index,
                int property_index)
{
    Player *player;
    Property *property;
    int cost;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player = &game->players[player_index];

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    if (!owns_complete_property_group(
            game,
            player_index,
            property->property_group))
        return 0;

    if (property->is_mortgaged)
        return 0;

    if (property->number_of_houses !=
        MAX_HOUSES_PER_PROPERTY)
        return 0;

    if (property->has_hotel)
        return 0;

    cost = property->current_hotel_construction_cost;

    if (player->cash < cost)
        return 0;

    player->cash -= cost;

    property->number_of_houses = 0;
    property->has_hotel = 1;

    for (i = 0; i < 4; i++)
    {
        property->buildings[i].building_type =
            NO_BUILDING;

        property->buildings[i].condition = 100;
    }

    property->buildings[4].building_type = HOTEL;
    property->buildings[4].condition = 100;

    return 1;
}






int mortgage_property(Game *game,
                      int player_index,
                      int property_index)
{
    Property *property;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    if (property->is_mortgaged)
        return 0;

    if (property->number_of_houses > 0 ||
        property->has_hotel)
        return 0;

    property->is_mortgaged = 1;

    game->players[player_index].cash +=
        property->current_mortgage_value;

    return 1;
}






int unmortgage_property(Game *game,
                        int player_index,
                        int property_index)
{
    Property *property;
    Player *player;
    int repayment;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    property = &game->properties[property_index];

    player = &game->players[player_index];

    if (property->owner_player_index != player_index)
        return 0;

    if (!property->is_mortgaged)
        return 0;

    repayment =
        property->current_mortgage_value * 110 / 100;

    if (player->cash < repayment)
        return 0;

    player->cash -= repayment;

    property->is_mortgaged = 0;

    return 1;
}






int calculate_maximum_loan(Game *game,
                           int player_index)
{
    int total_mortgage_value = 0;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    





    for (i = 0; i < NUMBER_OF_PROPERTIES; i++)
    {
        Property *property =
            &game->properties[i];

        if (property->owner_player_index ==
                player_index &&
            !property->is_mortgaged)
        {
            total_mortgage_value +=
                property->current_mortgage_value;
        }
    }

    



    return total_mortgage_value * 75 / 100;
}






int obtain_loan(Game *game,
                int player_index,
                int loan_amount)
{
    Player *player;
    Loan *loan;
    int maximum_loan;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (loan_amount <= 0)
        return 0;

    player = &game->players[player_index];

    loan = &player->player_loan;

    if (loan->is_active)
        return 0;

    maximum_loan =
        calculate_maximum_loan(
            game,
            player_index
        );

    if (loan_amount > maximum_loan)
        return 0;

    if (maximum_loan <= 0)
        return 0;

    loan->is_active = 1;

    loan->loan_amount = loan_amount;

    loan->outstanding_loan = loan_amount;

    loan->accrued_interest = 0;

    loan->interest_rate =
        game->current_loan_interest_rate;

    loan->rounds_remaining =
        LOAN_DURATION;

    



    loan->collateral_count = 0;

    




    for (i = 0;
         i < NUMBER_OF_PROPERTIES &&
         loan->collateral_count < 28;
         i++)
    {
        if (game->properties[i].owner_player_index ==
                player_index &&
            !game->properties[i].is_mortgaged)
        {
            loan->collateral_property_indices[
                loan->collateral_count
            ] = i;

            loan->collateral_count++;
        }
    }

    player->cash += loan_amount;

    return 1;
}






int repay_loan(Game *game,
               int player_index,
               int repayment_amount)
{
    Player *player;
    Loan *loan;
    int interest_payment;
    int principal_payment;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (repayment_amount <= 0)
        return 0;

    player = &game->players[player_index];

    loan = &player->player_loan;

    if (!loan->is_active)
        return 0;

    if (player->cash < repayment_amount)
        return 0;

    



    interest_payment =
        repayment_amount;

    if (interest_payment >
        loan->accrued_interest)
    {
        interest_payment =
            loan->accrued_interest;
    }

    loan->accrued_interest -=
        interest_payment;

    repayment_amount -=
        interest_payment;

    



    principal_payment =
        repayment_amount;

    if (principal_payment >
        loan->outstanding_loan)
    {
        principal_payment =
            loan->outstanding_loan;
    }

    loan->outstanding_loan -=
        principal_payment;

    player->cash -=
        interest_payment +
        principal_payment;

    



    if (loan->outstanding_loan <= 0 &&
        loan->accrued_interest <= 0)
    {
        loan->is_active = 0;

        loan->loan_amount = 0;

        loan->outstanding_loan = 0;

        loan->accrued_interest = 0;

        loan->rounds_remaining = 0;

        loan->collateral_count = 0;
    }

    return 1;
}






void accumulate_loan_interest(Game *game)
{
    int i;

    if (game == NULL)
        return;

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        Player *player =
            &game->players[i];

        Loan *loan =
            &player->player_loan;

        int interest;

        if (!loan->is_active)
            continue;

        




        interest =
            loan->outstanding_loan *
            loan->interest_rate / 100;

        loan->accrued_interest +=
            interest;

        if (loan->rounds_remaining > 0)
        {
            loan->rounds_remaining--;
        }
    }
}






int loan_default(Game *game,
                 int player_index)
{
    Player *player;
    Loan *loan;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    player = &game->players[player_index];

    loan = &player->player_loan;

    if (!loan->is_active)
        return 0;

    



    for (i = 0;
         i < loan->collateral_count;
         i++)
    {
        int property_index =
            loan->collateral_property_indices[i];

        if (property_index >= 0 &&
            property_index < NUMBER_OF_PROPERTIES)
        {
            Property *property =
                &game->properties[property_index];

            if (property->owner_player_index ==
                player_index)
            {
                property->owner_player_index =
                    BANK_OWNER;

                property->is_mortgaged = 0;

                property->number_of_houses = 0;

                property->has_hotel = 0;

                remove_property_from_player(
                    player,
                    property_index
                );
            }
        }
    }

    



    loan->is_active = 0;

    loan->loan_amount = 0;

    loan->outstanding_loan = 0;

    loan->accrued_interest = 0;

    loan->rounds_remaining = 0;

    loan->collateral_count = 0;

    return 1;
}






int purchase_insurance(Game *game,
                       int player_index,
                       int property_index,
                       InsuranceType insurance_type)
{
    Player *player;
    Property *property;
    int premium;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    if (insurance_type == NO_INSURANCE)
        return 0;

    player = &game->players[player_index];

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    if (property->insurance_policy.is_active)
        return 0;

    switch (insurance_type)
    {
        case BASIC_PROPERTY_INSURANCE:

            premium =
                property->current_property_value *
                5 / 100;

            break;

        case COMPREHENSIVE_INSURANCE:

            premium =
                property->current_property_value *
                10 / 100;

            break;

        case BUSINESS_INTERRUPTION_INSURANCE:

            if (!property->has_hotel)
                return 0;

            premium =
                property->current_property_value *
                15 / 100;

            break;

        default:
            return 0;
    }

    if (player->cash < premium)
        return 0;

    player->cash -= premium;

    property->insurance_policy.insurance_type =
        insurance_type;

    property->insurance_policy.property_index =
        property_index;

    property->insurance_policy.premium =
        premium;

    property->insurance_policy.rounds_remaining =
        INSURANCE_DURATION;

    property->insurance_policy.is_active = 1;

    return 1;
}






void update_insurance_policies(Game *game)
{
    int i;

    if (game == NULL)
        return;

    for (i = 0;
         i < NUMBER_OF_PROPERTIES;
         i++)
    {
        InsurancePolicy *policy =
            &game->properties[i].insurance_policy;

        if (!policy->is_active)
            continue;

        if (policy->rounds_remaining > 0)
            policy->rounds_remaining--;

        if (policy->rounds_remaining <= 0)
        {
            policy->is_active = 0;

            policy->insurance_type =
                NO_INSURANCE;

            policy->premium = 0;
        }
    }
}






int pay_tax(Game *game,
            int player_index,
            int tax_amount)
{
    Player *player;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (tax_amount < 0)
        return 0;

    player = &game->players[player_index];

    if (player->cash < tax_amount)
    {
        player->taxes_due +=
            tax_amount - player->cash;

        player->cash = 0;

        return 0;
    }

    player->cash -= tax_amount;

    return 1;
}






void update_property_age(Game *game)
{
    int i;

    if (game == NULL)
        return;

    for (i = 0;
         i < NUMBER_OF_PROPERTIES;
         i++)
    {
        if (game->properties[i].owner_player_index >= 0)
        {
            game->properties[i].property_age++;
        }
    }
}






void apply_property_depreciation(Game *game)
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

        if (property->owner_player_index < 0)
            continue;

        








        if (property->property_age > 50 &&
            property->property_age % 5 == 0)
        {
            if (property->depreciation_percentage < 30)
            {
                property->depreciation_percentage++;

                if (property->depreciation_percentage > 30)
                {
                    property->depreciation_percentage = 30;
                }

                property->current_property_value =
                    property->purchase_price *
                    (100 -
                     property->depreciation_percentage)
                    / 100;
            }
        }
    }
}






int calculate_maintenance_cost(Property *property)
{
    int cost = 0;

    if (property == NULL)
        return 0;

    




    if (property->number_of_houses > 0)
    {
        cost =
            property->number_of_houses *
            (property->current_house_construction_cost *
             5 / 100);
    }

    



    if (property->has_hotel)
    {
        cost =
            property->current_hotel_construction_cost *
            8 / 100;
    }

    return cost;
}






int maintain_property(Game *game,
                      int player_index,
                      int property_index)
{
    Player *player;
    Property *property;
    int cost;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player = &game->players[player_index];

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    cost =
        calculate_maintenance_cost(property);

    if (cost <= 0)
        return 0;

    if (player->cash < cost)
        return 0;

    player->cash -= cost;

    for (i = 0; i < 5; i++)
    {
        if (property->buildings[i].building_type !=
            NO_BUILDING)
        {
            property->buildings[i].condition = 100;
        }
    }

    property->is_damaged = 0;

    return 1;
}






int renovate_property(Game *game,
                      int player_index,
                      int property_index)
{
    Player *player;
    Property *property;
    int cost;
    int i;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    player = &game->players[player_index];

    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return 0;

    



    cost =
        property->current_property_value *
        10 / 100;

    if (player->cash < cost)
        return 0;

    player->cash -= cost;

    property->property_age = 0;

    property->depreciation_percentage = 0;

    property->current_property_value =
        property->purchase_price;

    for (i = 0; i < 5; i++)
    {
        if (property->buildings[i].building_type !=
            NO_BUILDING)
        {
            property->buildings[i].condition = 100;
        }
    }

    property->is_damaged = 0;

    return 1;
}






int calculate_auction_starting_bid(Property *property)
{
    if (property == NULL)
        return 0;

    



    return property->current_property_value *
           50 / 100;
}






int complete_auction(Game *game,
                     int property_index,
                     int winning_player_index,
                     int winning_bid)
{
    Property *property;
    Player *winner;

    if (game == NULL)
        return 0;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return 0;

    if (winning_player_index < 0 ||
        winning_player_index >= NUMBER_OF_PLAYERS)
        return 0;

    if (winning_bid <= 0)
        return 0;

    property = &game->properties[property_index];

    winner =
        &game->players[winning_player_index];

    if (property->owner_player_index != BANK_OWNER)
        return 0;

    if (winner->cash < winning_bid)
        return 0;

    if (!add_property_to_player(
            winner,
            property_index))
    {
        return 0;
    }

    winner->cash -= winning_bid;

    property->owner_player_index =
        winning_player_index;

    return 1;
}