#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_set>
#include <random>

#include "Point.h"
#include "Genotype.h"


namespace controller { class CellInteractor; }
namespace controller { class GameController; }
namespace genepool { class GenePool; }

namespace environment
{
    class Frame;
    class Cell; // Cell inherits Frame

    class Environment
    {
    private:
        const int WIDTH;
        const int HEIGHT;

        //upper limit of population for environment validation ( 2 = 200% of original population)
        const int population_upper_limit = 2;
        //TESTING ZONE
        // Added for step limitation in order to test out genepools
        const int max_step_count_before_reset = 5000;
        int cur_step_count = 0;

        std::vector<std::vector<Frame *>> frameMatrix;
        double time;    
        std::vector<Cell *> cells;
        genepool::GenePool* pool;
        controller::CellInteractor* interactor = nullptr;

    public:
        // Constructors / Destructors
        explicit Environment(int WIDTH, int HEIGHT, controller::CellInteractor* interactor = nullptr, genepool::GenePool* pool = nullptr);
        virtual ~Environment();

        // Enviroment Actions:
        // KISS 🥵😰
        void tickTime();
        bool isDay() const;
        bool isNight() const;
        double getTime() const;
        int getWidth() const;
        int getHeight() const;
        Frame* getFrame(const Point& point) const;
        /**
         * @brief Get the Cell object
         *
         * @param point Position on the map
         * @return Cell or nullptr if the Cell is missing
         */
        Cell* getCell(const Point& point) const;
        size_t getCellNumber() const;
        std::vector<Cell*>& getCells();



        void setCellInteractor(controller::CellInteractor* interactor);

        /**
         * @brief Updates Cell position in the frameMatrix
         *
         * @param cell Cell to update position
         * @param oldPos Cell's old position on the map
         */
       virtual void updateCellPosition(Cell* cell, const Point& oldPos);

        // Checks if a point has negative or larger coordinates than the map size 
        // On failure returns Point{-1, -1}
       bool checkPositionCorrectness(const Point& point) const;


        // Cell Actions:
        virtual Cell* AddCell(Cell* cell); // Call from logic
        virtual Cell* AddCell(const Point& point); // Call from controller
        virtual Cell* AddCell(const Point& point, std::vector<int> countOfWeights);
        virtual Cell* AddCell(const Point& point, genotype::Genotype* genotype);
        /**
         * @brief Removes the cell from the map
         *
         * @param cell
         */
        virtual void InvalidateCell(Cell* cell);

        /**
         * @brief Removes Cell object
         *
         * @param cell
         */
        virtual void RemoveCell(Cell* cell);

        void RemoveAllCells();

        // Cell Vision:
        /*
        00 01 02 03 04
        05 06 07 08 09
        10 11 😀 12 13
        14 15 16 17 18
        19 20 21 22 23
        */
        virtual std::vector<bool> getVisionField(const Point &point) const;

        // Returns a random correct and empty cell coordinate within a radius of one from the given point
        virtual Point randomFreePosition(const Point &point) const;
        // Return a random free poisiotn on map
        virtual Point getRandomFreePosOnMap(unsigned int attemp_count = 50) const;
        // Autospawns N cells
        virtual void generateCells(int N/*, controller::GameController *cont*/);

        /*
         * GenePool operations
         */
        //validates the environment by counting the number of cells
        virtual void ValidateEnvironment();
        //returns reference to GenePool
        genepool::GenePool* getGenePool();

        //return max cell count
        unsigned int getMaxCellCount();
    };

    class RandomGenerator {
    public:
        static int generateRandomIntNumber(int min, int max) {
            static std::random_device rd;
            static std::mt19937 rng(rd());
            std::uniform_int_distribution<int> idist(min, max);
            return idist(rng);
        }

        static double generateRandomDoubleNumber(double min, double max) {
            static std::random_device rd;
            static std::mt19937 rng(rd());
            std::uniform_real_distribution<double> ddist(min, max);
            return ddist(rng);
        }

        /**
         * @brief generateRandomPoint
         * @param top_left Top left corner of the bounding box
         * @param bottom_right Bottom right corner of the bounding box
         * @return Random point that does not go beyond the bounding box
         */
        static Point generateRandomPoint(const Point& top_left, const Point& bottom_right) {
            static std::random_device rd;
            static std::mt19937 rng(rd());

            std::uniform_int_distribution<int> xdist(top_left.j, bottom_right.j); // x
            std::uniform_int_distribution<int> ydist(top_left.i, bottom_right.i); // y


            return { ydist(rng), xdist(rng) };
        }
    };
}

#endif // ENVIRONMENT_H
