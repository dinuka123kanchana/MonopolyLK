#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

void initialize_board(Game *game);

void display_board(Game *game);

int move_player(Game *game,
                int player_index,
                int steps);

void initialize_players(Game *game);

void display_player_status(Game *game,
                           int player_index);

int check_bankruptcy(Game *game,
                     int player_index);

int player_wants_to_buy_property(Game *game, int player_index,
                                  int property_index);
void run_property_auction(Game *game, int property_index);
void player_attempt_insurance(Game *game, int player_index,
                               int property_index);
void player_attempt_loan_action(Game *game, int player_index);
void player_attempt_construction(Game *game, int player_index);

int purchase_property(Game *game,
                      int player_index,
                      int property_index);

int pay_property_rent(Game *game,
                      int player_index,
                      int property_index);

int calculate_property_rent(Property *property);

int calculate_utility_rent(Game *game,
                           int utility_index,
                           int dice_total);

int calculate_railway_rent(Game *game,
                           int railway_index);

int pay_tax(Game *game,
            int player_index,
            int tax_amount);

void accumulate_loan_interest(Game *game);

void update_insurance_policies(Game *game);

void update_property_age(Game *game);

void apply_property_depreciation(Game *game);

void initialize_national_event_cards(Game *game);

NationalEventCardType draw_national_event_card(
    Game *game);

void apply_national_event_card(
    Game *game,
    NationalEventCardType card);

void update_all_events(Game *game);

void handle_property_landing(Game *game,
                             int player_index,
                             int property_index);

void handle_railway_landing(Game *game,
                            int player_index,
                            int railway_index);

void handle_utility_landing(Game *game,
                            int player_index,
                            int utility_index);

void handle_event_square(Game *game,
                         int player_index);

void send_player_to_jail(Game *game,
                         int player_index);

int handle_jail_turn(Game *game,
                     int player_index,
                     Dice dice);

void process_landing(Game *game,
                     int player_index);

void end_round(Game *game);

int determine_winner(Game *game);

Dice roll_dice(void)
{
    Dice dice;

    dice.first_die = rand() % 6 + 1;
    dice.second_die = rand() % 6 + 1;

    dice.dice_total =
        dice.first_die + dice.second_die;

    dice.rolled_doubles =
        (dice.first_die == dice.second_die);

    return dice;
}

void initialize_game(Game *game)
{
    int i;

    if (game == NULL)
        return;

    game->current_round = 1;

    game->current_player_index = 0;

    game->game_is_over = 0;

    game->winner_player_index = -1;

    game->current_economic_condition =
        STABLE_ECONOMY;

    game->current_loan_interest_rate = 5;

    game->current_inflation.inflation_percentage = 0;

    game->property_market.boom_property_group =
        NO_PROPERTY_GROUP;

    game->property_market.decline_property_group =
        NO_PROPERTY_GROUP;

    game->property_market.boom_rounds_remaining = 0;

    game->property_market.decline_rounds_remaining = 0;

    game->property_market.boom_is_active = 0;

    game->property_market.decline_is_active = 0;

    game->current_economic_event.is_active = 0;

    game->current_economic_event.rounds_remaining = 0;

    game->current_government_regulation.is_active = 0;

    game->current_government_regulation.rounds_remaining = 0;

    game->current_regional_development.is_active = 0;

    game->current_regional_development.rounds_remaining = 0;

    initialize_board(game);

    initialize_players(game);

    game->players[0].player_strategy = AGGRESSIVE_INVESTOR;
    sprintf(game->players[0].player_name, "Aggressive Investor");

    game->players[1].player_strategy = CONSERVATIVE_BANKER;
    sprintf(game->players[1].player_name, "Conservative Banker");

    game->players[2].player_strategy = RISK_TAKER;
    sprintf(game->players[2].player_name, "Risk Taker");

    game->players[3].player_strategy = OPPORTUNISTIC_TRADER;
    sprintf(game->players[3].player_name, "Opportunistic Trader");

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        game->player_turn_order[i] = i;
    }

    initialize_national_event_cards(game);
}

void display_game_status(Game *game)
{
    int i;

    if (game == NULL)
        return;

    printf("\n");
    printf("========================================\n");
    printf("           MONOPOLY-LK STATUS\n");
    printf("========================================\n");

    printf("Round: %d / %d\n",
           game->current_round,
           MAX_ROUNDS);

    printf("Current Player: %d\n",
           game->current_player_index + 1);

    printf("Economic Condition: ");

    switch (game->current_economic_condition)
    {
        case STABLE_ECONOMY:
            printf("Stable Economy");
            break;

        case ECONOMIC_BOOM:
            printf("Economic Boom");
            break;

        case MODERATE_INFLATION:
            printf("Moderate Inflation");
            break;

        case HIGH_INFLATION:
            printf("High Inflation");
            break;

        case ECONOMIC_RECESSION:
            printf("Economic Recession");
            break;

        default:
            printf("Unknown");
            break;
    }

    printf("\n");

    printf("Inflation: %d%%\n",
           game->current_inflation.inflation_percentage);

    printf("Loan Interest Rate: %d%%\n",
           game->current_loan_interest_rate);

    printf("\n");

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        printf(
            "Player %d: %s | Cash = %d | Position = %d\n",
            i + 1,
            game->players[i].player_name,
            game->players[i].cash,
            game->players[i].current_board_position
        );
    }

    printf("========================================\n");
}

int move_player_by_dice(Game *game,
                        int player_index,
                        Dice dice)
{
    int old_position;
    int new_position;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    old_position =
        game->players[player_index]
        .current_board_position;

    new_position =
        old_position + dice.dice_total;

    if (new_position >= BOARD_SIZE)
    {
        new_position %= BOARD_SIZE;

        game->players[player_index].cash +=
            GO_REWARD;

        printf(
            "\n%s passed GO and received Rs. %d.\n",
            game->players[player_index].player_name,
            GO_REWARD
        );
    }

    game->players[player_index]
        .previous_board_position =
        old_position;

    game->players[player_index]
        .current_board_position =
        new_position;

    return 1;
}

void process_landing(Game *game,
                     int player_index)
{
    int position;
    BoardSquare *square;

    if (game == NULL)
        return;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return;

    position =
        game->players[player_index]
        .current_board_position;

    square =
        &game->board_squares[position];

    printf(
        "\n%s landed on %s.\n",
        game->players[player_index].player_name,
        square->square_name
    );

    switch (square->square_type)
    {
        case GO:

            printf("GO square.\n");

            break;

        case PROPERTY:

            handle_property_landing(
                game,
                player_index,
                square->property_index
            );

            break;

        case RAILWAY:

            handle_railway_landing(
                game,
                player_index,
                square->railway_index
            );

            break;

        case UTILITY:

            handle_utility_landing(
                game,
                player_index,
                square->utility_index
            );

            break;

        case TAX:

            printf(
                "Tax: Rs. %d\n",
                square->tax_amount
            );

            pay_tax(
                game,
                player_index,
                square->tax_amount
            );

            break;

        case EVENT:

            handle_event_square(
                game,
                player_index
            );

            break;

        case JAIL:

            printf("Just visiting Jail.\n");

            break;

        case FREE_PARKING:

            printf("Free Parking.\n");

            break;

        case GO_TO_JAIL:

            send_player_to_jail(
                game,
                player_index
            );

            break;

        case INSURANCE:
        {
            int i;
            Player *insured_player = &game->players[player_index];

            printf("Insurance square.\n");

            for (i = 0;
                 i < insured_player->number_of_owned_properties;
                 i++)
            {
                player_attempt_insurance(
                    game,
                    player_index,
                    insured_player->owned_property_indices[i]
                );
            }

            break;
        }

        case BANK:

            printf("Bank square.\n");

            player_attempt_loan_action(game, player_index);

            break;

        default:

            printf("Unknown board square.\n");

            break;
    }
}

void handle_property_landing(Game *game,
                             int player_index,
                             int property_index)
{
    Property *property;
    int owner_index;

    if (game == NULL)
        return;

    if (property_index < 0 ||
        property_index >= NUMBER_OF_PROPERTIES)
        return;

    property =
        &game->properties[property_index];

    owner_index =
        property->owner_player_index;

    if (owner_index == BANK_OWNER)
    {
        printf("Property is unowned.\n");

        printf(
            "Price: Rs. %d\n",
            property->current_property_value
        );

        if (player_wants_to_buy_property(
                game, player_index, property_index))
        {
            if (purchase_property(
                    game,
                    player_index,
                    property_index))
            {
                printf(
                    "%s purchased %s.\n",
                    game->players[player_index].player_name,
                    property->property_name
                );
            }
        }
        else
        {
            run_property_auction(game, property_index);
        }

        return;
    }

    if (owner_index == player_index)
    {
        printf("You own this property.\n");

        return;
    }

    printf(
        "Property belongs to %s.\n",
        game->players[owner_index].player_name
    );

    {
        int rent_amount = calculate_property_rent(property);

        if (pay_property_rent(game, player_index, property_index))
        {
            printf("Rent Paid : Rs. %d.\n", rent_amount);
            printf("Owner : %s.\n",
                   game->players[owner_index].player_name);
        }
    }
}

void handle_railway_landing(Game *game,
                            int player_index,
                            int railway_index)
{
    RailwayStation *railway;
    int owner_index;
    int rent;

    if (game == NULL)
        return;

    if (railway_index < 0 ||
        railway_index >= NUMBER_OF_RAILWAY_STATIONS)
        return;

    railway =
        &game->railway_stations[railway_index];

    owner_index =
        railway->owner_player_index;

    if (owner_index == BANK_OWNER)
    {
        if (game->players[player_index].cash >=
            railway->current_value)
        {
            game->players[player_index].cash -=
                railway->current_value;

            railway->owner_player_index =
                player_index;

            if (game->players[player_index]
                    .number_of_owned_railways <
                MAX_RAILWAYS_PER_PLAYER)
            {
                game->players[player_index]
                    .owned_railway_indices[
                        game->players[player_index]
                            .number_of_owned_railways
                    ] = railway_index;

                game->players[player_index]
                    .number_of_owned_railways++;
            }

            printf(
                "%s purchased %s.\n",
                game->players[player_index].player_name,
                railway->railway_name
            );
        }

        return;
    }

    if (owner_index == player_index)
    {
        printf("You own this railway.\n");

        return;
    }

    rent =
        calculate_railway_rent(
            game,
            railway_index
        );

    if (game->players[player_index].cash >= rent)
    {
        game->players[player_index].cash -= rent;

        game->players[owner_index].cash += rent;

        printf(
            "Railway rent of Rs. %d paid.\n",
            rent
        );
    }
    else
    {
        game->players[player_index].cash -= rent;

        game->players[owner_index].cash += rent;
    }
}

void handle_utility_landing(Game *game,
                            int player_index,
                            int utility_index)
{
    UtilityCompany *utility;
    int owner_index;
    int rent;
    Dice dice;

    if (game == NULL)
        return;

    if (utility_index < 0 ||
        utility_index >= NUMBER_OF_UTILITY_COMPANIES)
        return;

    utility =
        &game->utility_companies[utility_index];

    owner_index =
        utility->owner_player_index;

    if (owner_index == BANK_OWNER)
    {
        if (game->players[player_index].cash >=
            utility->current_value)
        {
            game->players[player_index].cash -=
                utility->current_value;

            utility->owner_player_index =
                player_index;

            if (game->players[player_index]
                    .number_of_owned_utilities <
                MAX_UTILITIES_PER_PLAYER)
            {
                game->players[player_index]
                    .owned_utility_indices[
                        game->players[player_index]
                            .number_of_owned_utilities
                    ] = utility_index;

                game->players[player_index]
                    .number_of_owned_utilities++;
            }

            printf(
                "%s purchased %s.\n",
                game->players[player_index].player_name,
                utility->utility_name
            );
        }

        return;
    }

    if (owner_index == player_index)
    {
        printf("You own this utility.\n");

        return;
    }

    dice = roll_dice();

    rent =
        calculate_utility_rent(
            game,
            utility_index,
            dice.dice_total
        );

    if (game->players[player_index].cash >= rent)
    {
        game->players[player_index].cash -= rent;

        game->players[owner_index].cash += rent;

        printf(
            "Utility rent of Rs. %d paid.\n",
            rent
        );
    }
    else
    {
        game->players[player_index].cash -= rent;

        game->players[owner_index].cash += rent;
    }
}

void handle_event_square(Game *game,
                         int player_index)
{
    NationalEventCardType card;

    (void)player_index;

    if (game == NULL)
        return;

    printf("\n");
    printf("A National Event Card was drawn.\n");

    card =
        draw_national_event_card(game);

    apply_national_event_card(
        game,
        card
    );

    printf("National event applied.\n");
}

void send_player_to_jail(Game *game,
                         int player_index)
{
    Player *player;

    if (game == NULL)
        return;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return;

    player =
        &game->players[player_index];

    player->current_board_position = 10;

    player->is_in_jail = 1;

    player->number_of_jail_turns = 0;

    player->jail_bail = 500;

    printf(
        "%s has been sent to Jail.\n",
        player->player_name
    );
}

int handle_jail_turn(Game *game,
                     int player_index,
                     Dice dice)
{
    Player *player;

    if (game == NULL)
        return 0;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return 0;

    player =
        &game->players[player_index];

    if (!player->is_in_jail)
        return 1;

    if (dice.rolled_doubles)
    {
        player->is_in_jail = 0;

        player->number_of_jail_turns = 0;

        printf(
            "%s rolled doubles and left Jail.\n",
            player->player_name
        );

        return 1;
    }

    player->number_of_jail_turns++;

    if (player->number_of_jail_turns >= 3)
    {
        if (player->cash >= player->jail_bail)
        {
            player->cash -=
                player->jail_bail;

            player->is_in_jail = 0;

            player->number_of_jail_turns = 0;

            printf(
                "%s paid bail and left Jail.\n",
                player->player_name
            );

            return 1;
        }
    }

    return 0;
}

int count_active_players(Game *game)
{
    int i;
    int count = 0;

    if (game == NULL)
        return 0;

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        if (game->players[i].player_status ==
            ACTIVE_PLAYER)
        {
            count++;
        }
    }

    return count;
}

int determine_winner(Game *game)
{
    int i;
    int active_players = 0;
    int winner = -1;

    if (game == NULL)
        return -1;

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        if (game->players[i].player_status ==
            ACTIVE_PLAYER)
        {
            active_players++;

            winner = i;
        }
    }

    if (active_players == 1)
    {
        game->winner_player_index = winner;

        game->game_is_over = 1;

        return winner;
    }

    return -1;
}

int get_next_player(Game *game,
                    int current_player)
{
    int i;
    int next_player;

    if (game == NULL)
        return -1;

    for (i = 1;
         i <= NUMBER_OF_PLAYERS;
         i++)
    {
        next_player =
            (current_player + i) %
            NUMBER_OF_PLAYERS;

        if (game->players[next_player]
                .player_status ==
            ACTIVE_PLAYER)
        {
            return next_player;
        }
    }

    return -1;
}

void end_round(Game *game)
{
    int i;

    if (game == NULL)
        return;

    printf("\n=============================================\n");
    printf("Round %d Summary\n", game->current_round);
    printf("=============================================\n");

    for (i = 0; i < NUMBER_OF_PLAYERS; i++)
    {
        Player *p = &game->players[i];
        int property_value_total = 0;
        int hotel_count = 0;
        int j;

        if (p->player_status == BANKRUPT_PLAYER)
        {
            printf("%s\nBANKRUPT\n", p->player_name);
            if (i < NUMBER_OF_PLAYERS - 1)
                printf("---------------------------------------------\n");
            continue;
        }

        for (j = 0; j < p->number_of_owned_properties; j++)
        {
            Property *prop =
                &game->properties[p->owned_property_indices[j]];

            property_value_total += prop->current_property_value;

            if (prop->has_hotel)
                hotel_count++;
        }

        printf("%s\n", p->player_name);
        printf("Cash : Rs. %d\n", p->cash);
        printf("Net Worth : Rs. %d\n",
               p->cash + property_value_total -
               p->player_loan.outstanding_loan);
        printf("Properties : %d\n", p->number_of_owned_properties);
        printf("Hotels : %d\n", hotel_count);

        if (p->player_loan.is_active)
        {
            printf("Outstanding Loan : Rs. %d\n",
                   p->player_loan.outstanding_loan);
        }
        else
        {
            printf("Outstanding Loan : None\n");
        }

        if (i < NUMBER_OF_PLAYERS - 1)
            printf("---------------------------------------------\n");
    }

    printf("=============================================\n");

    printf("\n");
    printf("========== END OF ROUND %d ==========\n",
           game->current_round);

    accumulate_loan_interest(game);

    update_insurance_policies(game);

    update_property_age(game);

    apply_property_depreciation(game);

    update_all_events(game);

    for (i = 0;
         i < NUMBER_OF_PLAYERS;
         i++)
    {
        if (game->players[i].player_status ==
            ACTIVE_PLAYER)
        {
            check_bankruptcy(
                game,
                i
            );
        }
    }

    if (determine_winner(game) != -1)
        return;

    game->current_round++;

    if (game->current_round > MAX_ROUNDS)
    {
        game->game_is_over = 1;

        {
            int highest_cash = -1;
            int highest_player = -1;

            for (i = 0;
                 i < NUMBER_OF_PLAYERS;
                 i++)
            {
                if (game->players[i].player_status ==
                    ACTIVE_PLAYER)
                {
                    if (game->players[i].cash >
                        highest_cash)
                    {
                        highest_cash =
                            game->players[i].cash;

                        highest_player = i;
                    }
                }
            }

            game->winner_player_index =
                highest_player;
        }
    }
}

void play_turn(Game *game,
               int player_index)
{
    Dice dice;
    Player *player;

    if (game == NULL)
        return;

    if (player_index < 0 ||
        player_index >= NUMBER_OF_PLAYERS)
        return;

    player =
        &game->players[player_index];

    if (player->player_status !=
        ACTIVE_PLAYER)
        return;

    printf("\n");
    printf("----------------------------------------\n");
    printf("Player Turn: %s\n",
           player->player_name);

    printf("Cash: Rs. %d\n",
           player->cash);

    dice = roll_dice();

    printf(
        "Dice: %d + %d = %d",
        dice.first_die,
        dice.second_die,
        dice.dice_total
    );

    if (dice.rolled_doubles)
        printf(" (DOUBLES)");

    printf("\n");

    if (player->is_in_jail)
    {
        if (!handle_jail_turn(
                game,
                player_index,
                dice))
        {
            printf(
                "%s remains in Jail.\n",
                player->player_name
            );

            printf("----------------------------------------\n");

            return;
        }
    }

    move_player_by_dice(
        game,
        player_index,
        dice
    );

    printf(
        "%s moved to board position %d.\n",
        player->player_name,
        player->current_board_position
    );

    process_landing(
        game,
        player_index
    );

    player_attempt_construction(
        game,
        player_index
    );

    check_bankruptcy(
        game,
        player_index
    );

    printf("----------------------------------------\n");
}

void play_game(Game *game)
{
    int players_completed_round;

    if (game == NULL)
        return;

    players_completed_round = 0;

    while (!game->game_is_over)
    {
        int current_player;

        current_player =
            game->current_player_index;

        if (game->players[current_player]
                .player_status ==
            ACTIVE_PLAYER)
        {
            play_turn(
                game,
                current_player
            );
        }

        if (determine_winner(game) != -1)
            break;

        players_completed_round++;

        game->current_player_index =
            get_next_player(
                game,
                current_player
            );

        if (players_completed_round >=
            count_active_players(game))
        {
            players_completed_round = 0;

            end_round(game);
        }

        if (game->current_player_index == -1)
        {
            game->game_is_over = 1;

            break;
        }
    }
}

void display_winner(Game *game)
{
    int winner;

    if (game == NULL)
        return;

    winner =
        game->winner_player_index;

    printf("\n");
    printf("========================================\n");
    printf("              GAME OVER\n");
    printf("========================================\n");

    if (winner >= 0 &&
        winner < NUMBER_OF_PLAYERS)
    {
        printf(
            "Winner: %s\n",
            game->players[winner].player_name
        );

        printf(
            "Final Cash: Rs. %d\n",
            game->players[winner].cash
        );
    }
    else
    {
        printf("No winner determined.\n");
    }

    printf("========================================\n");
}