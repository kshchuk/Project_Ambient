#include "cell.h"
#include "Environment.h"

namespace environment
{
    void Cell::move(int direction)
    {
        auto oldPos = position;

        switch (direction)
        {
        case 0:
            position.i--;
            break;
        case 1:
            position.i--;
            position.j++;
            break;
        case 2:
            position.j++;
            break;
        case 3:
            position.i++;
            position.j++;
            break;
        case 4:
            position.i++;
            break;
        case 5:
            position.i++;
            position.j--;
            break;
        case 6:
            position.j--;
            break;
        case 7:
            position.i--;
            position.j--;
            break;
        default:
            return;
        }
        currentEnergy -= kMoveCost;

        assert(environment != nullptr);
        environment->updateCellPosition(this, oldPos);

        if(currentEnergy <= 0)
        {
            this->totalScore -= 100;
            die();
            return;
        }
    }


    void Cell::evaluateAction(int action)
    {
        if(action <= 7)     // move
        {
            this->totalScore++;
        }

        else if(action == actions::kPhotosynthesis)
        {
            this->totalScore += 2;
        }

        else if(action <= 16) // attack
        {
            this->totalScore += 20;
        }
        else if(action == actions::kDuplication)
        {
            this->totalScore += 100;
        }
        else if(action == -1)
        {
            this->totalScore -= 1000;
        }
    }

    int foo(std::vector<double> values)
    {
        const int size = values.size();

        if(size == 0)                   // bad NN
            return -1;

        double sum = 0;

        for(int i = 0; i < size; i++)
        {
            assert(values[i] >= 0);
            sum += values[i];
        }

        double r = RandomGenerator::generateRandomDoubleNumber(0,sum);
        double temp = 0;

        for(int i = 0; i < size; i++)
        {
            temp += values[i];
            if(temp >= r)
                return i;
        }

        return 0;
    }

    int getBestOutputByProbabilyty(Matrix& outputs)
    {
        std::vector<int> indexes;
        std::vector<double> values;

        const int countOfOutputs = outputs.getY();

        for(int i = 0; i < countOfOutputs; i++)
        {
            if(outputs[i][0] > 0)
            {
                indexes.push_back(i);
                values.push_back(outputs[i][0]);
            }
        }

        const int index = foo(values);
        if(index == -1)
            return -1;
        return indexes[index];
    }

    bool Cell::cellCanMakeThisAction(Matrix& inputs, int& actionIndex)
    {

    }

    void Cell::removeBadOutputs(Matrix& inputs, Matrix& outputs)             // return count of > 0 outputs
    {
        int answer = 0;

        auto cellPosition = position;
        auto freePosition = environment->randomFreePosition(position);

        if (cellPosition.i == 0)
        {
            outputs[kMoveUp][0] = -1000000;
            outputs[kMoveUpRight][0] = -1000000;
            outputs[kMoveLeftUp][0] = -1000000;
        }
        if (cellPosition.j == 0)
        {
            outputs[kMoveLeftDown][0] = -1000000;
            outputs[kMoveLeft][0] = -1000000;
            outputs[kMoveLeftUp][0] = -1000000;
        }
        if(cellPosition.i == (environment->getHeight() - 1))
        {
            outputs[kMoveRightDown][0] = -1000000;
            outputs[kMoveDown][0] = -1000000;
            outputs[kMoveLeftDown][0] = -1000000;
        }
        if(cellPosition.j == (environment->getWidth() - 1))
        {
            outputs[kMoveRight][0] = -1000000;
            outputs[kMoveUpRight][0] = -1000000;
            outputs[kMoveRightDown][0] = -1000000;
        }
        if(inputs[6][0] == 1)
        {
            outputs[kMoveLeftUp][0] = -1000000;
        }
        else
        {
            outputs[kAttackLeftUp][0] = -1000000;
        }
        if(inputs[7][0] == 1)
        {
            outputs[kMoveUp][0] = -1000000;
        }
        else
        {
            outputs[kAttackUp][0] = -1000000;
        }
        if(inputs[8][0] == 1)
        {
            outputs[kMoveUpRight][0] = -1000000;
        }
        else
        {
            outputs[kAttackUpRight][0] = -1000000;
        }
        if(inputs[11][0] == 1)
        {
            outputs[kMoveLeft][0] = -1000000;
        }
        else
        {
            outputs[kAttackLeft][0] = -1000000;
        }
        if(inputs[12][0] == 1)
        {
            outputs[kMoveRight][0] = -1000000;
        }
        else
        {
            outputs[kAttackRight][0] = -1000000;
        }
        if(inputs[15][0] == 1)
        {
            outputs[kMoveLeftDown][0] = -1000000;
        }
        else
        {
            outputs[kAttackLeftDown][0] = -1000000;
        }
        if(inputs[16][0] == 1)
        {
            outputs[kMoveDown][0] = -1000000;
        }
        else
        {
            outputs[kAttackDown][0] = -1000000;
        }
        if(inputs[17][0] == 1)
        {
            outputs[kMoveRightDown][0] = -1000000;
        }
        else
        {
            outputs[kAttackRightDown][0] = -1000000;
        }
        if(delayAfterDuplication > 0)
        {
             outputs[kAttackLeftUp][0] = -1000000;
             outputs[kAttackUp][0] = -1000000;
             outputs[kAttackUpRight][0] = -1000000;
             outputs[kAttackLeft][0] = -1000000;
             outputs[kAttackRight][0] = -1000000;
             outputs[kAttackLeftDown][0] = -1000000;
             outputs[kAttackDown][0] = -1000000;
             outputs[kAttackRightDown][0] = -1000000;
        }
        if(inputs[24][0] < 0.95)                         // can Photosynthesis if energy is low than 95 procent, can duplicate if  energy is more than 95 proc
        {
            outputs[kDuplication][0] = -1000000;
        }
        else
        {
            outputs[kPhotosynthesis][0] = -1000000;
        }
        if(freePosition.i == -1)
        {
            outputs[kDuplication][0] = -1000000;
        }

        //const int countOfIterations = outputs.getY();
        //for(int i = 0; i < countOfIterations; i++)
        //    if(outputs[i][0] > 0)
        //        answer++;
        //return answer;

    }

    int Cell::bestPossibleChoiceIndex(Matrix& outputs, Matrix& inputs)
    {
        removeBadOutputs(inputs, outputs);
        return getBestOutputByProbabilyty(outputs);

        //Point cellPosition = position;
        //Point freePosition = environment->randomFreePosition(position);
        /*
        switch (index)
        {
            case kMoveUp:
                if (cellPosition.i == 0 || inputs[7][0] == 1) {
                    outputs[kMoveUp][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveUpRight:
                if (cellPosition.i == 0 || cellPosition.j == (environment->getWidth() - 1) || inputs[8][0] == 1)
                {
                    outputs[kMoveUpRight][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveRight:
                if (cellPosition.j == (environment->getWidth() - 1) || inputs[12][0] == 1) {
                    outputs[kMoveRight][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveRightDown:
                if (cellPosition.i == (environment->getHeight() - 1) || cellPosition.j == (environment->getWidth() - 1) || inputs[17][0] == 1) {
                    outputs[kMoveRightDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveDown:
                if (cellPosition.i == (environment->getHeight() - 1) || inputs[16][0] == 1) {
                    outputs[kMoveDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveLeftDown:
                if (cellPosition.i == (environment->getHeight() - 1) || cellPosition.j == 0 || inputs[15][0] == 1) {
                    outputs[kMoveLeftDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveLeft:
                if (cellPosition.j == 0 || inputs[11][0] == 1) {
                    outputs[kMoveLeft][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kMoveLeftUp:
                if (cellPosition.i == 0 || cellPosition.j == 0 || inputs[6][0] == 1) {
                    outputs[kMoveLeftUp][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackUp:
                if(inputs[7][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackUp][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackUpRight:
                if(inputs[8][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackUpRight][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackRight:
                if(inputs[12][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackRight][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackRightDown:
                if(inputs[17][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackRightDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackDown:
                if(inputs[16][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackLeftDown:
                if(inputs[15][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackLeftDown][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackLeft:
                if(inputs[11][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackLeft][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kAttackLeftUp:
                if(inputs[6][0] == 0 || delayAfterDuplication > 0)
                {
                    outputs[kAttackLeftUp][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kDuplication:
                //freePosition = environment->randomFreePosition(position);

                if (inputs[24][0] < 0.8 || freePosition.i == -1)
                {
                    outputs[kDuplication][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
            case kPhotosynthesis:
                if (inputs[24][0] >= 0.99 )                     // can Photosynthesis if energy is low than 99 procent
                {
                    outputs[kPhotosynthesis][0] = -1000000;
                    return bestPossibleChoiceIndex(outputs, inputs);
                }
                return index;
                default:
                        return index;
        }

        */
    }

    int Cell::makeChoice(Matrix& inputs)
    {
        const int countOfIteration = this->genotype.getCountOfLayers();
        Matrix temp = genotype::ReLU(genotype.getWeightsMatrixByIndex(0) * inputs + genotype.getBaesMatrixByIndex(0));
        for(int i = 1; i < countOfIteration; i++)
        {
            temp = genotype::ReLU(genotype.getWeightsMatrixByIndex(i) * temp + genotype.getBaesMatrixByIndex(i));
        }
        return bestPossibleChoiceIndex(temp, inputs);
    }

    void increaseEnergy(double &currentEnergy, double maxEnergy, double count)
    {
        currentEnergy += count;
        if (currentEnergy > maxEnergy)
            currentEnergy = maxEnergy;
    }

    void Cell::photosynthesis()
    {
        increaseEnergy(currentEnergy, maxEnergy, kPhotosynthesisAdd);
        stepsCount--;

        if(stepsCount == 0)
        {
            if(aggressiveness >= 0.05)
                aggressiveness -= 0.05;
            stepsCount = kSteps;
        }
    }

    void Cell::attack(int action)
    {
        Point currentPosition = position;

        if(action == kAttackUp)
        {
            currentPosition.i--;
        }
        else if(action == kAttackUpRight)
        {
            currentPosition.i--;
            currentPosition.j++;
        }
        else if(action == kAttackRight)
        {
            currentPosition.j++;
        }
        else if(action == kAttackRightDown)
        {
            currentPosition.i++;
            currentPosition.j++;
        }
        else if(action == kAttackDown)
        {
            currentPosition.i++;
        }
        else if(action == kAttackLeftDown)
        {
            currentPosition.i++;
            currentPosition.j--;
        }
        else if(action == kAttackLeft)
        {
            currentPosition.j--;
        }
        else
        {
            currentPosition.i--;
            currentPosition.j--;
        }

        Cell* opponent = environment->getCell(currentPosition);
        double opponentEnergy = opponent->getCurrentEnergy();
        opponentEnergy -= (currentEnergy * kAttackCoefficient + minDamage);
        currentEnergy -= currentEnergy * kAttackCost;
        opponent->setCurrentEnergy(opponentEnergy);

        aggressiveness += 0.05;
        stepsCount = kSteps;

        if (aggressiveness >= 1)
            aggressiveness = 0.99;

        if (opponentEnergy <= 0)
        {
            opponent->currentEnergy -= 40;          //&&&&&&&&&&&
            opponent->die();
            increaseEnergy(currentEnergy, maxEnergy, kPrise);
        }
    }

    void Cell::duplicate()
    {
        Point freePosition = environment->randomFreePosition(position);
        currentEnergy = (currentEnergy - kDuplicationCost) * 0.5;
        delayAfterDuplication = kDelay;
        this->environment->AddCell(new Cell(*this, freePosition));
    }

    Cell::Cell(Point startingPosition, Environment* environment)
        : Frame(startingPosition, environment)
    {
        genotype = genotype::Genotype(std::vector<int>{10,10,10,10,10});
        aggressiveness = RandomGenerator::generateRandomDoubleNumber(0,1);
        maxEnergy = RandomGenerator::generateRandomIntNumber(kMinEnergy,kMaxEnergy);
        currentEnergy = maxEnergy;
    }

    Cell::Cell(Cell &mother, Point freePosition) : Frame(mother)
    {
        genotype = genotype::Genotype(mother.getGenotype());
        aggressiveness = mother.getAggressiveness();
        currentEnergy = mother.getCurrentEnergy();
        maxEnergy = mother.getMaxEnergy();
        position = freePosition;
    }

    // countOfWeights - Additional vector, which contain countOfWeights in i's layer(from 1 to n-1)
    Cell::Cell(Point startingPosition, std::vector<int> countOfWeights, Environment* environment): Frame(startingPosition, environment)
    {
        genotype = genotype::Genotype(countOfWeights);
        aggressiveness = RandomGenerator::generateRandomDoubleNumber(0,1);
        maxEnergy = RandomGenerator::generateRandomIntNumber(kMinEnergy,kMaxEnergy);
        currentEnergy = maxEnergy;
    }

    double Cell::getAggressiveness() const
    {
        return aggressiveness;
    }

    double Cell::getCurrentEnergy() const
    {
        return currentEnergy;
    }

    double Cell::getMaxEnergy() const
    {
        return maxEnergy;
    }

    genotype::Genotype Cell::getGenotype() const
    {
        return this->genotype;
    }

    bool Cell::isAlive() const
    {
        return isAliveStatus;
    }

    int Cell::getStepsCount() const
    {
        return this->stepsCount;
    }
    int Cell::getTotalScore() const
    {
        return this->totalScore;
    }
    actions Cell::act() //std::vector<double> inputs
    {

        if (numberOfMovesToDeath == 0) // Remove or delete cell
        {
            this->totalScore -= 20;
            die();
            return kCellIsDead;
        }

        numberOfMovesToDeath--;
        if(delayAfterDuplication > 0)
            delayAfterDuplication--;

        std::vector<double> inputs;
        std::vector<bool> vision;
        assert(environment!=nullptr);
        vision = environment->getVisionField(position);

        for(int i = 0; i < vision.size(); i++)
        {
            inputs.push_back(vision[i]);
        }

        inputs.push_back(currentEnergy / maxEnergy);
        inputs.push_back(aggressiveness);
        Matrix mInputs(1, this->genotype.getWeightsMatrixByIndex(0).getX());
        mInputs.addColumn(inputs);

        int indexOfAction = makeChoice(mInputs);

        evaluateAction(indexOfAction);

        if (indexOfAction < 0)
        {
            std::cout << "negative action!" << std::endl;
        }

        if (indexOfAction < kPhotosynthesis)                  //  0 - 7 <- 8 directions for moving
        {
            move(indexOfAction);
            return kMoveUp;
        }

        else if (indexOfAction == kPhotosynthesis)
        {
            photosynthesis();
            return kPhotosynthesis;
        }

        else if (indexOfAction == kDuplication)
        {
            duplicate();
            return kDuplication;
        }
        else if (indexOfAction == -1)
        {
            die();
            return kCellIsDead;
        }
        else
        {
            attack(indexOfAction);
            return kAttackUp;
        }
    }
}
