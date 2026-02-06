#include <iostream>

int test_update_small_deformation();
int test_gap_relax();
int test_heartbeat();
int test_reset();
int test_invariant_memory();
int test_invariant_kappa_nonnegative();
int test_invariant_metric_positivity();
int test_invariant_tau();
int test_invariant_morphology();
int test_invariant_regime_irreversible();
int test_invariant_collapse_geometry();
int test_invariant_forbidden_domain();
int test_algorithm_delta();
int test_algorithm_phi();
int test_algorithm_kappa_decay();
int test_algorithm_det_g();
int test_algorithm_tau();
int test_update_medium_stress();
int test_update_high_stress();
int test_gap_sequence_stability();
int test_heartbeat_high_memory();
int test_reset_in_REL();
int test_reset_after_collapse();
int test_regime_ACC_to_DEV();
int test_regime_DEV_to_REL();
int test_regime_REL_to_COL();
int test_regime_cycle_ACC_DEV_REL();
int test_collapse_trigger_boundary();
int test_collapse_under_gap();
int test_collapse_under_heartbeat();
int test_collapse_after_stress_sequence();
int test_numeric_reject_NaN();
int test_numeric_reject_INF();
int test_numeric_reject_denormals();
int test_numeric_underflow_overflow_safety();
int test_determinism_single_run();
int test_determinism_multi_run();
int test_determinism_no_hidden_state();
void test_bridge_numeric_reject_NaN();

int main()
{

    if (test_update_small_deformation() != 0)
    {
        std::cerr << "test_update_small_deformation FAILED\n";
        return 1;
    }

    if (test_gap_relax() != 0)
    {
        std::cerr << "test_gap_relax FAILED\n";
        return 1;
    }

    if (test_heartbeat() != 0)
    {
        std::cerr << "test_heartbeat FAILED\n";
        return 1;
    }

    if (test_reset() != 0)
{
    std::cerr << "test_reset FAILED\n";
    return 1;
}

if (test_invariant_memory() != 0) {
    std::cerr << "test_invariant_memory FAILED\n";
    return 1;
}

if (test_invariant_kappa_nonnegative() != 0) {
    std::cerr << "test_invariant_kappa_nonnegative FAILED\n";
    return 1;
}

if (test_invariant_metric_positivity() != 0) {
    std::cerr << "test_invariant_metric_positivity FAILED\n";
    return 1;
}

if (test_invariant_tau() != 0) {
    std::cerr << "test_invariant_tau FAILED\n";
    return 1;
}

if (test_invariant_morphology() != 0) {
    std::cerr << "test_invariant_morphology FAILED\n";
    return 1;
}

if (test_invariant_regime_irreversible() != 0) {
    std::cerr << "test_invariant_regime_irreversible FAILED\n";
    return 1;
}

if (test_invariant_collapse_geometry() != 0) {
    std::cerr << "test_invariant_collapse_geometry FAILED\n";
    return 1;
}

if (test_invariant_forbidden_domain() != 0) {
    std::cerr << "test_invariant_forbidden_domain FAILED\n";
    return 1;
}

if (test_algorithm_delta() != 0)
{
    std::cerr << "test_algorithm_delta FAILED\n";
    return 1;
}

if (test_algorithm_phi() != 0)
{
    std::cerr << "test_algorithm_phi FAILED\n";
    return 1;
}

if (test_algorithm_kappa_decay() != 0)
{
    std::cerr << "test_algorithm_kappa_decay FAILED\n";
    return 1;
}

if (test_algorithm_det_g() != 0)
{
    std::cerr << "test_algorithm_det_g FAILED\n";
    return 1;
}

if (test_algorithm_tau() != 0)
{
    std::cerr << "test_algorithm_tau FAILED\n";
    return 1;
}

if (test_update_medium_stress() != 0) return 1;
if (test_update_high_stress() != 0) return 1;
if (test_gap_sequence_stability() != 0) return 1;

if (test_heartbeat_high_memory() != 0) {
    std::cerr << "test_heartbeat_high_memory FAILED\n";
    return 1;
}

if (test_reset_in_REL() != 0) return 1;
if (test_reset_after_collapse() != 0) return 1;

if (test_regime_ACC_to_DEV() != 0) {
    std::cerr << "ACC_to_DEV FAILED\n";
    return 1;
}
if (test_regime_DEV_to_REL() != 0) {
    std::cerr << "DEV_to_REL FAILED\n";
    return 1;
}
if (test_regime_REL_to_COL() != 0) {
    std::cerr << "REL_to_COL FAILED\n";
    return 1;
}
if (test_regime_cycle_ACC_DEV_REL() != 0) {
    std::cerr << "ACC_DEV_REL_cycle FAILED\n";
    return 1;
}

if (test_collapse_trigger_boundary() != 0) {
    std::cerr << "collapse_trigger_boundary FAILED\n";
    return 1;
}
if (test_collapse_under_gap() != 0) {
    std::cerr << "collapse_under_gap FAILED\n";
    return 1;
}
if (test_collapse_under_heartbeat() != 0) {
    std::cerr << "collapse_under_heartbeat FAILED\n";
    return 1;
}
if (test_collapse_after_stress_sequence() != 0) {
    std::cerr << "collapse_after_stress_sequence FAILED\n";
    return 1;
}
if (test_numeric_reject_NaN() != 0) return 1;
if (test_numeric_reject_INF() != 0) return 1;
if (test_numeric_reject_denormals() != 0) return 1;
if (test_numeric_underflow_overflow_safety() != 0) return 1;
if (test_determinism_single_run() != 0) return 1;
if (test_determinism_multi_run() != 0) return 1;
if (test_determinism_no_hidden_state() != 0) return 1;

std::printf("Running bridge_numeric_reject_NaN...\n");
test_bridge_numeric_reject_NaN();

    std::cout << "ALL TESTS PASSED\n";
    return 0; 
}
