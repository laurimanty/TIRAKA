// Datastructures.cc
//
// Student name: Lauri Mänty

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    for (auto town : townMap)
    {
        delete town.second;
        town.second = nullptr;
    }
}

unsigned int Datastructures::town_count()
{
    // Replace the line below with your implementation
    return townMap.size();
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    townMap.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{

    if (townMap.find(id) != townMap.end())
        {
            return false;
        }
    else
        {
            Town* newTown = new Town;
            newTown->town_id_ = id;
            newTown->town_name_ = name;
            newTown->town_coord_ = coord;
            newTown->town_tax_ = tax;
            townMap.insert({id, newTown});
            return true;
        }

}

Name Datastructures::get_town_name(TownID id)
{
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        return it->second->town_name_;
    }
    else
    {
        return NO_NAME;
    }
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        return it->second->town_coord_;
    }
    else
    {
        return NO_COORD;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        return it->second->town_tax_;
    }
    else
    {
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> idVec;
    std::unordered_map<TownID,Town*>::const_iterator it;
    for(auto it=townMap.begin(); it!=townMap.end(); it++)
    {
        idVec.push_back(it->first);
    }
    return idVec;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{

    std::vector<TownID> nameVec;
    std::unordered_map<TownID,Town*>::const_iterator it;
    for(auto it=townMap.begin(); it!=townMap.end(); it++)
    {
        if (it->second->town_name_ == name)
        {
            nameVec.push_back(it->first);
        }
    }
    return nameVec;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        it->second->town_name_ = newname;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> idVec = all_towns();
    std::sort(idVec.begin(), idVec.end(),
               [&](TownID a, TownID b) { return get_town_name(a) < get_town_name(b); });
    return idVec;

}

int Datastructures::eucDistSqr(Coord coord1, Coord coord2 /*={0,0}*/)
{
    int dx = (coord1.x-coord2.x);
    int dy = (coord1.y-coord2.y);
    int distSqr = dx*dx + dy*dy;
    return distSqr; // returning square of the distance is more efficient
                    // and for sorting purposes its exactly same as returning square root
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> idVec = all_towns();
    std::sort(idVec.begin(), idVec.end(),
               [this](TownID a, TownID b) {
        return eucDistSqr(get_town_coordinates(a))
                < eucDistSqr(get_town_coordinates(b)); });
    return idVec;
}

TownID Datastructures::min_distance()
{
    std::vector<TownID> idVec = towns_distance_increasing();
    return idVec.front();
}

TownID Datastructures::max_distance()
{
    std::vector<TownID> idVec = towns_distance_increasing();
    return idVec.back();
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    std::unordered_map<TownID,Town*>::const_iterator itV;
    std::unordered_map<TownID,Town*>::const_iterator itM;

    itV = townMap.find(vassalid);
    itM = townMap.find(masterid);
    if (itV != townMap.end() && itM != townMap.end() && itV->second->master_ == nullptr)
    {
        itV->second->master_ = itM->second;
        itM->second->vassals_.insert(std::make_pair(vassalid,itV->second));
        return true;
    }
    else
    {
        return false  ;
    }

}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> vassalVec;
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
       for(auto town : it->second->vassals_)
       {
           vassalVec.push_back(town.second->town_id_);
       }
    }
    else
    {
        vassalVec.push_back(NO_TOWNID);
    }
    return vassalVec;
}

std::vector<TownID> Datastructures::taxer_path_recursive(Town* masterTown,std::vector<TownID>& masterVec)
{
    if(masterTown->master_ != nullptr)
    {
        masterVec.push_back(masterTown->master_->town_id_);
        taxer_path_recursive(masterTown->master_, masterVec);
        return masterVec;
    }
    else
    {
        return masterVec;
    }
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> masterVec;
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        if(it->second->master_ != nullptr)
        {
            masterVec.push_back(it->second->master_->town_id_);
            taxer_path_recursive(it->second->master_, masterVec);
            return masterVec;
        }
        else
        {
            return masterVec;
        }
    }
    else
    {
        masterVec.push_back(NO_TOWNID);
        return masterVec;
    }
}
 // optional
bool Datastructures::remove_town(TownID id)
{
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        if(it->second->master_ != nullptr) // on mestari
        {
            if(it->second->vassals_.size() != 0) // on vassaali ja mestari
            {
                for(auto vassal : it->second->vassals_) // käy läpi vassaalit
                {
                    vassal.second->master_ = nullptr; //vassaalin mestari nollaten
                    // lisää vassaalin mestariksi mestarin mestari
                    add_vassalship(vassal.second->town_id_, it->second->master_->town_id_);
                }
            }
            //Behold..! My spaget!
            std::unordered_map<TownID,Town*>::const_iterator it2;
            it2 = it->second->master_->vassals_.find(id);
            it->second->master_->vassals_.erase(it2);
        }
        else //ei mestaria
        {
            if(it->second->vassals_.size() != 0) // on vassaali ja ei mestaria
            {
                for(auto vassal : it->second->vassals_) // käy läpi vassaalit
                {
                    vassal.second->master_ = nullptr; // be free
                }
            }
        }
        townMap.erase(id);
        return true;
    }
    else
    {
        return false;
    }

}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> idVec = all_towns();
    std::sort(idVec.begin(), idVec.end(),
               [this, coord](TownID a, TownID b) {
        return eucDistSqr(get_town_coordinates(a),coord)
                < eucDistSqr(get_town_coordinates(b),coord); });
    return idVec;
}

std::vector<TownID> Datastructures::longest_path_recursive(Town* Town,
                                                           std::vector<TownID>& tempPath,
                                                           std::vector<TownID>& longestPath)
{
    tempPath.push_back(Town->town_id_);
    if(!Town->vassals_.empty())
    {
        for(auto vassal : Town->vassals_)
        {
            longest_path_recursive(vassal.second, tempPath, longestPath);
            tempPath.pop_back();
        }
    }
    else
    {
        if(tempPath.size() > longestPath.size())
        {
            longestPath = tempPath;
        }
    }
    return longestPath;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> longestPath;
    std::vector<TownID> tempPath;
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if (it != townMap.end())
    {
        if(it->second->vassals_.empty())
        {
            longestPath.push_back(NO_TOWNID);
            return longestPath;
        }
        else
        {
            longest_path_recursive(it->second, tempPath ,longestPath);
            return longestPath;
        }
    }
    else
    {
        longestPath.push_back(NO_TOWNID);
        return longestPath;
    }
}

int Datastructures::total_net_tax_recursive(Town* masterTown)
{
    int tax = masterTown->town_tax_;
    if(!masterTown->vassals_.empty())
    {
       for(auto vassal : masterTown->vassals_)
       {
           tax += floor(total_net_tax_recursive(vassal.second)*0.1);
       }
    }
    return tax;
}

int Datastructures::total_net_tax(TownID id)
{
    int tax;
    std::unordered_map<TownID,Town*>::const_iterator it;
    it = townMap.find(id);
    if(it != townMap.end())
    {
            tax = total_net_tax_recursive(it->second);
            if(it->second->master_ != nullptr)
            {
                return floor(tax*0.9);
            }
            else
            {
                return tax;
            }
    }
    else
    {
        return NO_VALUE;
    }

}
