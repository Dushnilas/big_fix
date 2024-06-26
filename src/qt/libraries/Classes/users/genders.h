#ifndef GENDERS_H
#define GENDERS_H

#include <string>
#include <vector>
#include <map>


enum class Gender
{
    Undefined,
    Male,
    Female,
    Walmart_Bag,
    Croissant,
    Helicopter_Apache,
    Shiba_Inu,
    Pepe_the_Frog,
    Distracted_Boyfriend,
    Success_Kid,
    Skill_Issue,
    Grumpy_Cat,
    Rickrolling,
    Big_Chungus,
    Karen,
    Michael_Jordan,
    Mocking_SpongeBob,
    Daewoo_Matiz,
    Galaxy_Brain,
    Wojak,
    Cat_Vibing_to_Music,
    Doctor,
    Surprised_Pikachu,
    Expanding_Brain,
    Nyan_Cat,
    _28_Udarov_Nozhom,
    Ermahgerd_Girl,
    Disaster_Girl,
    This_is_Fine_Dog,
    Kermit_sipping_tea,
    Pyaterochka_Packet,
    Sad_Keanu,
    Arthurs_Fist,
    SpongeGar,
    Storozheva,
    Harambe,
    Y_U_No_Guy,
    Philosoraptor,
    Condescending_Wonka,
    Mr_Beast,
    Bad_Luck_Brian,
    Hide_the_Pain_Harold,
    Dat_Boi,
    Pavel_Popov,
    Robert_Patission,
    Geralt_Of_Rivia,
    Penza,
    Planta_MF_3B_Smart_Compression,
    Avenger,
    Porshe_Panamera_Turbo
};

const std::map<Gender, std::string> genderMap = {
    {Gender::Male, "Male"},
    {Gender::Female, "Female"},
    {Gender::Walmart_Bag, "Walmart Bag"},
    {Gender::Croissant, "Croissant"},
    {Gender::Helicopter_Apache, "Helicopter Apache"},
    {Gender::Shiba_Inu, "Shiba Inu"},
    {Gender::Pepe_the_Frog, "Pepe the Frog"},
    {Gender::Distracted_Boyfriend, "Distracted Boyfriend"},
    {Gender::Success_Kid, "Success Kid"},
    {Gender::Skill_Issue, "Skill Issue"},
    {Gender::Grumpy_Cat, "Grumpy Cat"},
    {Gender::Rickrolling, "Rickrolling"},
    {Gender::Big_Chungus, "Big Chungus"},
    {Gender::Karen, "Karen"},
    {Gender::Michael_Jordan, "Michael Jordan"},
    {Gender::Mocking_SpongeBob, "Mocking SpongeBob"},
    {Gender::Daewoo_Matiz, "Daewoo Matiz"},
    {Gender::Galaxy_Brain, "Galaxy Brain"},
    {Gender::Wojak, "Wojak"},
    {Gender::Cat_Vibing_to_Music, "Cat Vibing to Music"},
    {Gender::Doctor, "Doctor"},
    {Gender::Surprised_Pikachu, "Surprised Pikachu"},
    {Gender::Expanding_Brain, "Expanding Brain"},
    {Gender::Nyan_Cat, "Nyan Cat"},
    {Gender::_28_Udarov_Nozhom, "28 Udarov Nozhom"},
    {Gender::Ermahgerd_Girl, "Ermahgerd Girl"},
    {Gender::Disaster_Girl, "Disaster Girl"},
    {Gender::This_is_Fine_Dog, "This is Fine Dog"},
    {Gender::Kermit_sipping_tea, "Kermit sipping tea"},
    {Gender::Pyaterochka_Packet, "Pyaterochka Packet"},
    {Gender::Sad_Keanu, "Sad Keanu"},
    {Gender::Arthurs_Fist, "Arthurs Fist"},
    {Gender::SpongeGar, "SpongeGar"},
    {Gender::Storozheva, "Storozheva"},
    {Gender::Harambe, "Harambe"},
    {Gender::Y_U_No_Guy, "Y U No Guy"},
    {Gender::Philosoraptor, "Philosoraptor"},
    {Gender::Condescending_Wonka, "Condescending Wonka"},
    {Gender::Mr_Beast, "Mr Beast"},
    {Gender::Bad_Luck_Brian, "Bad Luck Brian"},
    {Gender::Hide_the_Pain_Harold, "Hide the Pain Harold"},
    {Gender::Dat_Boi, "Dat Boi"},
    {Gender::Pavel_Popov, "Pavel Popov"},
    {Gender::Robert_Patission, "Robert Patission"},
    {Gender::Geralt_Of_Rivia, "Geralt Of Rivia"},
    {Gender::Penza, "Penza"},
    {Gender::Planta_MF_3B_Smart_Compression, "Planta MF 3B Smart Compression"},
    {Gender::Avenger, "Avenger"},
    {Gender::Porshe_Panamera_Turbo, "Porshe Panamera Turbo"},
    {Gender::Undefined, "Undefined"}
};

std::string genderToString(Gender gender);

std::vector<std::string> getAllGenders();

std::map<std::string, Gender> ReverseMap();

Gender strToGender(const std::string &genderStr);

#endif
