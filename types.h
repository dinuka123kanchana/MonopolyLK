#ifndef TYPES_H
#define TYPES_H

#define BOARD_SIZE 40
#define NUMBER_OF_PLAYERS 4
#define MAX_ROUNDS 500

#define INITIAL_PLAYER_CASH 30000
#define GO_REWARD 2000

#define NUMBER_OF_PROPERTIES 22
#define NUMBER_OF_RAILWAY_STATIONS 4
#define NUMBER_OF_UTILITY_COMPANIES 2

#define MAX_PROPERTIES_PER_PLAYER 22
#define MAX_RAILWAYS_PER_PLAYER 4
#define MAX_UTILITIES_PER_PLAYER 2

#define MAX_HOUSES_PER_PROPERTY 4

#define LOAN_DURATION 20
#define INSURANCE_DURATION 20

#define NUMBER_OF_NATIONAL_EVENT_CARDS 20

#define AUCTION_BID_INCREMENT 250

#define BANK_OWNER -1
#define NO_OWNER -1

typedef enum
{
    GO,
    PROPERTY,
    EVENT,
    TAX,
    RAILWAY,
    UTILITY,
    JAIL,
    FREE_PARKING,
    GO_TO_JAIL,
    INSURANCE,
    BANK
} BoardSquareType;

typedef enum
{
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE,
    NO_PROPERTY_GROUP
} PropertyGroup;

typedef enum
{
    AGGRESSIVE_INVESTOR,
    CONSERVATIVE_BANKER,
    RISK_TAKER,
    OPPORTUNISTIC_TRADER
} PlayerStrategy;

typedef enum
{
    NO_INSURANCE,
    BASIC_PROPERTY_INSURANCE,
    COMPREHENSIVE_INSURANCE,
    BUSINESS_INTERRUPTION_INSURANCE
} InsuranceType;

typedef enum
{
    NO_BUILDING,
    HOUSE,
    HOTEL
} BuildingType;

typedef enum
{
    ACTIVE_PLAYER,
    BANKRUPT_PLAYER
} PlayerStatus;

typedef enum
{
    STABLE_ECONOMY,
    ECONOMIC_BOOM,
    MODERATE_INFLATION,
    HIGH_INFLATION,
    ECONOMIC_RECESSION
} EconomicCondition;

typedef enum
{
    TOURISM_BOOM,
    FUEL_CRISIS,
    HEAVY_MONSOON,
    ECONOMIC_RECESSION_EVENT,
    STOCK_MARKET_BOOM,
    GOVERNMENT_HOUSING_PROGRAMME,
    FOREIGN_INVESTMENT,
    POLITICAL_UNREST
} EconomicEventType;

typedef enum
{
    INCREASE_PROPERTY_TAX,
    REDUCE_LOAN_INTEREST,
    HOUSING_SUBSIDY,
    LUXURY_PROPERTY_TAX,
    RAILWAY_MODERNIZATION,
    ELECTRICITY_TARIFF_REVISION,
    INSURANCE_REGULATION,
    ANTI_SPECULATION_ACT
} GovernmentRegulationType;

typedef enum
{
    SOUTHERN_TOURISM_BOOM,
    PORT_CITY_EXPANSION,
    IT_INDUSTRY_GROWTH,
    NORTHERN_DEVELOPMENT_PROGRAMME,
    TEA_EXPORT_BOOM,
    AIRPORT_EXPANSION,
    UNIVERSITY_CITY_GROWTH,
    BEACH_POLLUTION,
    FLOOD_DAMAGE,
    TRANSPORT_STRIKE,
    ELECTRICITY_TARIFF_INCREASE,
    WATER_SHORTAGE
} RegionalDevelopmentType;

typedef enum
{
    TOURISM_HYPE,
    FUEL_SHORTAGE,
    HEAVY_FLOODS,
    POLITICAL_RALLY,
    STOCK_MARKET_RISE,
    ECONOMIC_DOWNTURN,
    HOUSING_SUBSIDY_CARD,
    INTEREST_RATE_CUT,
    INTEREST_RATE_INCREASE,
    TAX_AMNESTY,
    POWER_FAILURE,
    FOREIGN_FUNDING,
    PORT_EXPANSION,
    FESTIVAL_SEASON,
    LABOUR_STRIKE,
    INSURANCE_DISCOUNT,
    PROPERTY_REVALUATION,
    CURRENCY_DEPRECIATION,
    GOVERNMENT_GRANT,
    NATIONAL_DISASTER
} NationalEventCardType;

typedef enum
{
    NO_DISASTER,
    FIRE,
    FLOOD,
    RIOT,
    BUILDING_COLLAPSE,
    ELECTRICAL_FAILURE
} DisasterType;

typedef struct
{
    InsuranceType insurance_type;
    int property_index;
    int premium;
    int rounds_remaining;
    int is_active;
} InsurancePolicy;

typedef struct
{
    BuildingType building_type;
    int condition;
} Building;

typedef struct
{
    char property_name[50];

    int board_index;
    PropertyGroup property_group;

    int purchase_price;
    int mortgage_value;
    int base_rent;
    int house_construction_cost;
    int hotel_construction_cost;

    int current_property_value;
    int current_mortgage_value;
    int current_rent;
    int current_house_construction_cost;
    int current_hotel_construction_cost;

    int owner_player_index;

    int is_mortgaged;

    int number_of_houses;
    int has_hotel;
    Building buildings[5];

    int property_age;
    int depreciation_percentage;

    InsurancePolicy insurance_policy;

    int is_damaged;
    int repair_cost;
} Property;

typedef struct
{
    char railway_name[50];

    int board_index;
    int owner_player_index;
    int mortgage_value;
    int current_value;
    int is_mortgaged;
} RailwayStation;

typedef struct
{
    char utility_name[50];

    int board_index;
    int owner_player_index;
    int mortgage_value;
    int current_value;
    int is_mortgaged;
} UtilityCompany;

typedef struct
{
    int is_active;
    int loan_amount;
    int outstanding_loan;
    int accrued_interest;
    int interest_rate;
    int rounds_remaining;

    int collateral_property_indices[28];
    int collateral_count;
} Loan;

typedef struct
{
    int board_index;
    char square_name[50];
    BoardSquareType square_type;

    int property_index;
    int railway_index;
    int utility_index;

    int tax_amount;
} BoardSquare;

typedef struct
{
    int player_index;
    char player_name[50];
    PlayerStrategy player_strategy;
    PlayerStatus player_status;

    int cash;
    int taxes_due;

    int current_board_position;
    int previous_board_position;

    int is_in_jail;
    int number_of_jail_turns;
    int jail_bail;

    int owned_property_indices[MAX_PROPERTIES_PER_PLAYER];
    int number_of_owned_properties;

    int owned_railway_indices[MAX_RAILWAYS_PER_PLAYER];
    int number_of_owned_railways;

    int owned_utility_indices[MAX_UTILITIES_PER_PLAYER];
    int number_of_owned_utilities;

    Loan player_loan;

    int bankruptcy_round;
} Player;

typedef struct
{
    int first_die;
    int second_die;
    int dice_total;
    int rolled_doubles;
} Dice;

typedef struct
{
    int property_index;
    int opening_bid;
    int current_highest_bid;
    int highest_bidder_index;
    int auction_is_active;
} Auction;

typedef struct
{
    int inflation_percentage;
} Inflation;

typedef struct
{
    EconomicEventType economic_event_type;
    int rounds_remaining;
    int is_active;
} EconomicEvent;

typedef struct
{
    GovernmentRegulationType government_regulation_type;
    int rounds_remaining;
    int is_active;
} GovernmentRegulation;

typedef struct
{
    RegionalDevelopmentType regional_development_type;
    int rounds_remaining;
    int is_active;
} RegionalDevelopment;

typedef struct
{
    int boom_property_group;
    int decline_property_group;
    int boom_rounds_remaining;
    int decline_rounds_remaining;
    int boom_is_active;
    int decline_is_active;
} PropertyMarket;

typedef struct
{
    BoardSquare board_squares[BOARD_SIZE];

    Property properties[NUMBER_OF_PROPERTIES];

    RailwayStation railway_stations[NUMBER_OF_RAILWAY_STATIONS];

    UtilityCompany utility_companies[NUMBER_OF_UTILITY_COMPANIES];

    Player players[NUMBER_OF_PLAYERS];

    int player_turn_order[NUMBER_OF_PLAYERS];

    int current_round;
    int current_player_index;
    int game_is_over;
    int winner_player_index;

    EconomicCondition current_economic_condition;
    int current_loan_interest_rate;
    Inflation current_inflation;
    PropertyMarket property_market;

    EconomicEvent current_economic_event;
    GovernmentRegulation current_government_regulation;
    RegionalDevelopment current_regional_development;

    NationalEventCardType national_event_cards[NUMBER_OF_NATIONAL_EVENT_CARDS];

    int current_national_event_card_index;

} Game;

#endif