//
// Created by Kdzumba on 2023/06/10.
//

#ifndef WAVE_FUNCTION_COLLAPSE_SUDOKUBLOCK_H
#define WAVE_FUNCTION_COLLAPSE_SUDOKUBLOCK_H


#include <vector>
#include <memory>
#include "BlockState.h"
#include <QObject>

class SudokuBlock :public QObject
{
    Q_OBJECT
public:
    SudokuBlock();
    ~SudokuBlock() override;
    std::vector<std::unique_ptr<BlockState>>& get_available_states();
    [[nodiscard]] const std::unique_ptr<BlockState>& get_collapsed_state() const;
    void set_collapsed_state(std::unique_ptr<BlockState> option);
    void set_coordinate(int x, int y);
    std::tuple<int,int> get_coordinate();
    void remove_state(const std::unique_ptr<BlockState>& option);
    void make_current_block(bool is_current);
    bool operator< (const SudokuBlock& other) const;
    bool operator== (const SudokuBlock& other) const;
    [[nodiscard]] unsigned int get_entropy() const;
    [[nodiscard]] SudokuBlock* get_previous_block() const;
    void set_previous_block(SudokuBlock* previous);
    void add_available_state(const std::unique_ptr<BlockState>& option);
    void set_available_states(const std::vector<std::unique_ptr<BlockState>>& options);
    [[nodiscard]] bool get_is_permanently_collapsed() const;
    void set_permanently_collapsed(bool is_collapsed);
    void reset_available_states();
    bool is_current_block() const {return m_is_current_block;}
public slots:
    void should_show_superpositions(bool show);
signals:
    void block_collapsed(int phase);
    void show_superpositions(bool show);

private:
    SudokuBlock* m_previous;
    bool m_is_current_block;
    std::unique_ptr<BlockState> m_collapsed_state;
    std::tuple<int, int> m_coordinate;
    std::vector<std::unique_ptr<BlockState>> m_available_states;
    bool m_is_permanently_collapsed;
};


#endif //WAVE_FUNCTION_COLLAPSE_SUDOKUBLOCK_H
