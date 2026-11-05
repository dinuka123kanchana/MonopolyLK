#include <stdio.h>
#include <string.h>

#include "types.h"

void initialize_players(Game *game);
void initialize_player(Player *player, int player_index);
void update_player_position(Player *player, int new_position);
void release_player_from_jail(Player *player);
int add_player_cash(Player *player, int amount);
int remove_player_cash(Player *player, int amount);

int add_property_to_player(Player *player, int property_index);
int remove_property_from_player(Player *player, int property_index);

int add_railway_to_player(Player *player, int railway_index);
int remove_railway_from_player(Player *player, int railway_index);

int add_utility_to_player(Player *player, int utility_index);
int remove_utility_from_player(Player *player, int utility_index);

void bankrupt_player(Player *player, int round_number);

int check_bankruptcy(Game *game, int player_index);

static int is_property_loan_locked(Player *player, int property_index);

int player_wants_to_buy_property(Game *game, int player_index, int property_index);
void run_property_auction(Game *game, int property_index);
void player_attempt_insurance(Game *game, int player_index, int property_index);
void player_attempt_loan_action(Game *game, int player_index);
void player_attempt_construction(Game *game, int player_index);

void initialize_players(Game *game)
{
    if (game == NULL)
    {
        return;
    }

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        initialize_player(&game->players[i], i);
        game->player_turn_order[i] = i;
    }

    game->current_player_index = 0;
}

void initialize_player(Player *player, int player_index)
{
    if (player == NULL)
    {
        return;
    }

    player->player_index = player_index;

    sprintf(player->player_name, "Player %d", player_index + 1);

    player->player_strategy = CONSERVATIVE_BANKER;

    player->player_status = ACTIVE_PLAYER;

    player->cash = INITIAL_PLAYER_CASH;

    player->taxes_due = 0;

    player->current_board_position = 0;
    player->previous_board_position = 0;

    player->is_in_jail = 0;
    player->number_of_jail_turns = 0;
    player->jail_bail = 0;

    player->number_of_owned_properties = 0;

    for (int i = 0; i < MAX_PROPERTIES_PER_PLAYER; i++)
    {
        player->owned_property_indices[i] = -1;
    }

    player->number_of_owned_railways = 0;

    for (int i = 0; i < MAX_RAILWAYS_PER_PLAYER; i++)
    {
        player->owned_railway_indices[i] = -1;
    }

    player->number_of_owned_utilities = 0;

    for (int i = 0; i < MAX_UTILITIES_PER_PLAYER; i++)
    {
        player->owned_utility_indices[i] = -1;
    }

    player->player_loan.is_active = 0;
    player->player_loan.loan_amount = 0;
    player->player_loan.outstanding_loan = 0;
    player->player_loan.accrued_interest = 0;
    player->player_loan.interest_rate = 0;
    player->player_loan.rounds_remaining = 0;
    player->player_loan.collateral_count = 0;

    for (int i = 0; i < 28; i++)
    {
        player->player_loan.collateral_property_indices[i] = -1;
    }

    player->bankruptcy_round = -1;
}

void update_player_position(Player *player, int new_position)
{
    if (player == NULL)
    {
        return;
    }

    if (new_position < 0 || new_position >= BOARD_SIZE)
    {
        return;
    }

    player->previous_board_position =
        player->current_board_position;

    player->current_board_position = new_position;
}

void release_player_from_jail(Player *player)
{
    if (player == NULL)
    {
        return;
    }

    player->is_in_jail = 0;
    player->number_of_jail_turns = 0;
    player->jail_bail = 0;
}

int add_player_cash(Player *player, int amount)
{
    if (player == NULL)
    {
        return 0;
    }

    if (amount < 0)
    {
        return 0;
    }

    player->cash += amount;

    return 1;
}

int remove_player_cash(Player *player, int amount)
{
    if (player == NULL)
    {
        return 0;
    }

    if (amount < 0)
    {
        return 0;
    }

    if (player->cash < amount)
    {
        return 0;
    }

    player->cash -= amount;

    return 1;
}

int add_property_to_player(Player *player, int property_index)
{
    if (player == NULL)
    {
        return 0;
    }

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_properties;
         i++)
    {
        if (player->owned_property_indices[i] == property_index)
        {
            return 0;
        }
    }

    if (player->number_of_owned_properties >=
        MAX_PROPERTIES_PER_PLAYER)
    {
        return 0;
    }

    player->owned_property_indices[
        player->number_of_owned_properties
    ] = property_index;

    player->number_of_owned_properties++;

    return 1;
}

int remove_property_from_player(Player *player, int property_index)
{
    int found_index = -1;

    if (player == NULL)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_properties;
         i++)
    {
        if (player->owned_property_indices[i] == property_index)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        return 0;
    }

    for (int i = found_index;
         i < player->number_of_owned_properties - 1;
         i++)
    {
        player->owned_property_indices[i] =
            player->owned_property_indices[i + 1];
    }

    player->owned_property_indices[
        player->number_of_owned_properties - 1
    ] = -1;

    player->number_of_owned_properties--;

    return 1;
}

int add_railway_to_player(Player *player, int railway_index)
{
    if (player == NULL)
    {
        return 0;
    }

    if (railway_index < 0 ||
        railway_index >= NUMBER_OF_RAILWAY_STATIONS)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_railways;
         i++)
    {
        if (player->owned_railway_indices[i] == railway_index)
        {
            return 0;
        }
    }

    if (player->number_of_owned_railways >=
        MAX_RAILWAYS_PER_PLAYER)
    {
        return 0;
    }

    player->owned_railway_indices[
        player->number_of_owned_railways
    ] = railway_index;

    player->number_of_owned_railways++;

    return 1;
}

int remove_railway_from_player(Player *player, int railway_index)
{
    int found_index = -1;

    if (player == NULL)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_railways;
         i++)
    {
        if (player->owned_railway_indices[i] == railway_index)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        return 0;
    }

    for (int i = found_index;
         i < player->number_of_owned_railways - 1;
         i++)
    {
        player->owned_railway_indices[i] =
            player->owned_railway_indices[i + 1];
    }

    player->owned_railway_indices[
        player->number_of_owned_railways - 1
    ] = -1;

    player->number_of_owned_railways--;

    return 1;
}

int add_utility_to_player(Player *player, int utility_index)
{
    if (player == NULL)
    {
        return 0;
    }

    if (utility_index < 0 ||
        utility_index >= NUMBER_OF_UTILITY_COMPANIES)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_utilities;
         i++)
    {
        if (player->owned_utility_indices[i] == utility_index)
        {
            return 0;
        }
    }

    if (player->number_of_owned_utilities >=
        MAX_UTILITIES_PER_PLAYER)
    {
        return 0;
    }

    player->owned_utility_indices[
        player->number_of_owned_utilities
    ] = utility_index;

    player->number_of_owned_utilities++;

    return 1;
}

int remove_utility_from_player(Player *player, int utility_index)
{
    int found_index = -1;

    if (player == NULL)
    {
        return 0;
    }

    for (int i = 0;
         i < player->number_of_owned_utilities;
         i++)
    {
        if (player->owned_utility_indices[i] == utility_index)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        return 0;
    }

    for (int i = found_index;
         i < player->number_of_owned_utilities - 1;
         i++)
    {
        player->owned_utility_indices[i] =
            player->owned_utility_indices[i + 1];
    }

    player->owned_utility_indices[
        player->number_of_owned_utilities - 1
    ] = -1;

    player->number_of_owned_utilities--;

    return 1;
}

void bankrupt_player(Player *player, int round_number)
{
    if (player == NULL)
    {
        return;
    }

    player->player_status = BANKRUPT_PLAYER;
    player->bankruptcy_round = round_number;
    player->is_in_jail = 0;
    player->number_of_jail_turns = 0;
}

static int is_property_loan_locked(Player *player, int property_index)
{
    int i;

    if (player == NULL)
    {
        return 0;
    }

    if (!player->player_loan.is_active)
    {
        return 0;
    }

    for (i = 0; i < player->player_loan.collateral_count; i++)
    {
        if (player->player_loan.collateral_property_indices[i] ==
            property_index)
        {
            return 1;
        }
    }

    return 0;
}

int check_bankruptcy(Game *game, int player_index)
{
    Player *player;
    int liabilities;
    int available_assets;
    int i;

    if (game == NULL)
    {
        return 0;
    }

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
    {
        return 0;
    }

    player = &game->players[player_index];

    if (player->player_status == BANKRUPT_PLAYER)
    {
        return 1;
    }

    liabilities = player->taxes_due;

    if (liabilities <= 0)
    {
        return 0;
    }

    available_assets = player->cash;

    for (i = 0; i < player->number_of_owned_properties; i++)
    {
        int property_index =
            player->owned_property_indices[i];

        Property *property =
            &game->properties[property_index];

        if (!property->is_mortgaged &&
            !is_property_loan_locked(player, property_index))
        {
            available_assets +=
                property->current_mortgage_value;
        }
    }

    for (i = 0; i < player->number_of_owned_railways; i++)
    {
        RailwayStation *railway =
            &game->railway_stations[
                player->owned_railway_indices[i]];

        if (!railway->is_mortgaged)
        {
            available_assets += railway->mortgage_value;
        }
    }

    for (i = 0; i < player->number_of_owned_utilities; i++)
    {
        UtilityCompany *utility =
            &game->utility_companies[
                player->owned_utility_indices[i]];

        if (!utility->is_mortgaged)
        {
            available_assets += utility->mortgage_value;
        }
    }

    if (available_assets >= liabilities)
    {
        return 0;
    }

    bankrupt_player(player, game->current_round);

    printf(
        "%s has been declared bankrupt.\n",
        player->player_name
    );

    return 1;
}

int obtain_loan(Game *game, int player_index, int loan_amount);
int repay_loan(Game *game, int player_index, int repay_amount);
int calculate_maximum_loan(Game *game, int player_index);
int purchase_insurance(Game *game, int player_index,
                        int property_index, InsuranceType insurance_type);
int build_house(Game *game, int player_index, int property_index);
int build_hotel(Game *game, int player_index, int property_index);
int owns_complete_property_group(Game *game, int player_index,
                                  PropertyGroup property_group);
int calculate_auction_starting_bid(Property *property);
int complete_auction(Game *game, int property_index,
                      int winning_player_index, int winning_bid);

int player_wants_to_buy_property(Game *game, int player_index,
                                  int property_index)
{
    Player *player;
    Property *property;
    int price;

    if (game == NULL)
        return 0;

    player = &game->players[player_index];
    property = &game->properties[property_index];
    price = property->current_property_value;

    if (player->cash < price)
        return 0;

    switch (player->player_strategy)
    {
        case AGGRESSIVE_INVESTOR:
        case RISK_TAKER:
            return (player->cash - price) >= 0;

        case CONSERVATIVE_BANKER:
            return (player->cash - price) >=
                   (player->cash / 2);

        case OPPORTUNISTIC_TRADER:
        default:
            return price <= (player->cash * 40 / 100);
    }
}

void run_property_auction(Game *game, int property_index)
{
    Property *property;
    int current_bid;
    int highest_bidder;
    int i;
    int active_bidders;

    if (game == NULL)
        return;

    property = &game->properties[property_index];

    current_bid = calculate_auction_starting_bid(property);
    highest_bidder = -1;

    printf("\nAuction Started.\n");
    printf("Property : %s\n", property->property_name);
    printf("Opening Bid : Rs. %d\n", current_bid);

    active_bidders = 1;

    while (active_bidders > 0)
    {
        active_bidders = 0;

        for (i = 0; i < NUMBER_OF_PLAYERS; i++)
        {
            Player *bidder = &game->players[i];
            int max_they_will_pay;

            if (bidder->player_status != ACTIVE_PLAYER)
                continue;

            switch (bidder->player_strategy)
            {
                case AGGRESSIVE_INVESTOR:
                    max_they_will_pay =
                        property->current_property_value *
                        120 / 100;
                    break;

                case RISK_TAKER:
                    max_they_will_pay = bidder->cash;
                    break;

                case OPPORTUNISTIC_TRADER:
                    max_they_will_pay =
                        property->current_property_value *
                        90 / 100;
                    break;

                case CONSERVATIVE_BANKER:
                default:
                    max_they_will_pay =
                        property->current_property_value *
                        95 / 100;
                    break;
            }

            if (bidder->cash > current_bid + AUCTION_BID_INCREMENT &&
                current_bid + AUCTION_BID_INCREMENT <=
                    max_they_will_pay)
            {
                current_bid += AUCTION_BID_INCREMENT;
                highest_bidder = i;
                active_bidders++;

                printf(
                    "%s bids Rs. %d.\n",
                    bidder->player_name,
                    current_bid
                );
            }
        }
    }

    if (highest_bidder == -1)
    {
        printf("No player bids. Ownership remains with the Bank.\n");
        return;
    }

    if (complete_auction(game, property_index,
                          highest_bidder, current_bid))
    {
        printf(
            "%s wins the auction.\n",
            game->players[highest_bidder].player_name
        );
    }
}

void player_attempt_insurance(Game *game, int player_index,
                               int property_index)
{
    Player *player;
    Property *property;
    InsuranceType chosen_type;

    if (game == NULL)
        return;

    player = &game->players[player_index];
    property = &game->properties[property_index];

    if (property->owner_player_index != player_index)
        return;

    if (property->insurance_policy.is_active)
        return;

    switch (player->player_strategy)
    {
        case AGGRESSIVE_INVESTOR:
            if (property->has_hotel)
            {
                chosen_type = COMPREHENSIVE_INSURANCE;
            }
            else
            {
                chosen_type = BASIC_PROPERTY_INSURANCE;
            }
            break;

        case CONSERVATIVE_BANKER:
            chosen_type = COMPREHENSIVE_INSURANCE;
            break;

        case OPPORTUNISTIC_TRADER:
            if (property->current_property_value < 5000)
                return;
            chosen_type = COMPREHENSIVE_INSURANCE;
            break;

        case RISK_TAKER:
        default:
            return;
    }

    if (property->number_of_houses == 0 && !property->has_hotel)
        return;

    if (purchase_insurance(game, player_index,
                            property_index, chosen_type))
    {
        char policy_name[50];

        if (chosen_type == BASIC_PROPERTY_INSURANCE)
        {
            strcpy(policy_name, "Basic Property Insurance");
        }
        else if (chosen_type == COMPREHENSIVE_INSURANCE)
        {
            strcpy(policy_name, "Comprehensive Insurance");
        }
        else
        {
            strcpy(policy_name, "Business Interruption Insurance");
        }

        printf(
            "%s purchased.\nProperty : %s\nPremium : Rs. %d\n",
            policy_name,
            property->property_name,
            property->insurance_policy.premium
        );
    }
}

void player_attempt_loan_action(Game *game, int player_index)
{
    Player *player;
    int max_loan;

    if (game == NULL)
        return;

    player = &game->players[player_index];

    if (player->player_loan.is_active)
    {
        if (player->player_strategy == CONSERVATIVE_BANKER &&
            player->cash > player->player_loan.outstanding_loan)
        {
            repay_loan(game, player_index,
                       player->player_loan.outstanding_loan);
        }
        return;
    }

    max_loan = calculate_maximum_loan(game, player_index);

    if (max_loan <= 0)
        return;

    switch (player->player_strategy)
    {
        case RISK_TAKER:
            if (obtain_loan(game, player_index, max_loan))
                printf(
                    "%s obtained a secured loan. Loan Amount : Rs. %d\n",
                    player->player_name, max_loan
                );
            break;

        case AGGRESSIVE_INVESTOR:
            if (max_loan / 2 > 0 &&
                obtain_loan(game, player_index, max_loan / 2))
                printf(
                    "%s obtained a secured loan. Loan Amount : Rs. %d\n",
                    player->player_name, max_loan / 2
                );
            break;

        case OPPORTUNISTIC_TRADER:
            if (max_loan / 4 > 0 &&
                obtain_loan(game, player_index, max_loan / 4))
                printf(
                    "%s obtained a secured loan. Loan Amount : Rs. %d\n",
                    player->player_name, max_loan / 4
                );
            break;

        case CONSERVATIVE_BANKER:
        default:
            if (player->cash < 1000 && max_loan / 4 > 0 &&
                obtain_loan(game, player_index, max_loan / 4))
                printf(
                    "%s obtained a secured loan. Loan Amount : Rs. %d\n",
                    player->player_name, max_loan / 4
                );
            break;
    }
}

void player_attempt_construction(Game *game, int player_index)
{
    Player *player;
    int i;

    if (game == NULL)
        return;

    player = &game->players[player_index];

    if (player->player_strategy == CONSERVATIVE_BANKER &&
        player->player_loan.is_active)
        return;

    for (i = 0; i < player->number_of_owned_properties; i++)
    {
        int property_index = player->owned_property_indices[i];
        Property *property = &game->properties[property_index];

        if (!owns_complete_property_group(
                game, player_index, property->property_group))
            continue;

        if (property->has_hotel)
            continue;

        if (property->number_of_houses < MAX_HOUSES_PER_PROPERTY)
        {
            if (build_house(game, player_index, property_index))
            {
                printf(
                    "%s constructed a house on %s.\n",
                    player->player_name,
                    property->property_name
                );
            }
        }
        else
        {
            if (build_hotel(game, player_index, property_index))
            {
                printf(
                    "%s upgraded %s to a Hotel.\n",
                    player->player_name,
                    property->property_name
                );
            }
        }
    }
}