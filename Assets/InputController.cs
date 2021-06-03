using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class InputController : MonoBehaviour, PlayerInputAction.IPlayerActions
{
    PlayerInputAction playerInputAction;
    public PlayerController playerController;

    void Awake()
    {
        playerInputAction = new PlayerInputAction();
        playerInputAction.Player.SetCallbacks(this);
    }

    void OnEnable()
    {
        playerInputAction.Player.Enable();
    }

    void OnDisable()
    {
        playerInputAction.Player.Disable();
    }

    public void OnMove(InputAction.CallbackContext context)
    {

        if (context.started)
        {
            return;
        }

        Vector2 axis = context.ReadValue<Vector2>();
        playerController.SetAxis(axis);

    }

    public void OnJump(InputAction.CallbackContext context)
    {
        if (context.started)
        {
            playerController.TryJump();
        }
    }
}
